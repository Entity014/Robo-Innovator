import cv2
import numpy as np
from imutils.perspective import four_point_transform

cap = cv2.VideoCapture(2)

width, height = 800, 600
lower_color = np.array([72, 59, 33])
upper_color = np.array([143, 255, 101])

cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)


def scanMission(Lower, frame, debug=False):
    global mission_contour
    global max_area

    mission_contour = np.array([[0, 0], [width, 0], [width, height], [0, height]])
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = cv2.Canny(blur, 50, 150)
    mask = cv2.dilate(edges, np.ones((5, 5)), iterations=1)
    # _, threshold = cv2.threshold(blur, Lower, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv2.contourArea, reverse=True)

    max_area = 0
    if debug:
        cv2.imshow("test2", mask)
    for contour in contours:
        area = cv2.contourArea(contour)
        x, y, w, h = cv2.boundingRect(contour)
        if area >= 1e5:
            peri = cv2.arcLength(contour, True)
            approx = cv2.approxPolyDP(contour, 0.015 * peri, True)
            if area > max_area and len(approx) == 4:
                mission_contour = approx
                cv2.putText(
                    frame,
                    f" {area}",
                    (x, y),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 100, 255),
                    2,
                )
                max_area = area

    # cv2.drawContours(frame, [mission_contour], -1, (0, 255, 0), 3)


def matrix(Loop_x, Loop_y, frame):
    x, y = 0, 0
    for i in range(Loop_x):
        for j in range(Loop_y):
            new = frame[
                y + (j * int(height / 3)) : y + int(height / 3) + (j * int(height / 3)),
                x + (i * int(width / 5)) : x + int(width / 5) + (i * int(width / 5)),
            ]
            if j == 0:
                binaryDetection(3, 1, new)
            elif j == 1:
                shapeDetection(new, i)
            else:
                pass


def shapeDetection(frame, i):
    shape = ""
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = cv2.Canny(blur, 50, 150)
    # mask = cv2.dilate(edges, np.ones((3, 3)), iterations=1)
    edges[:20, :] = 0
    edges[-30:, :] = 0
    edges[:, :20] = 0
    edges[:, -15:] = 0
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for contour in contours:
        area = cv2.contourArea(contour)
        x, y, w, h = cv2.boundingRect(contour)
        peri = cv2.arcLength(contour, True)
        approx = cv2.approxPolyDP(contour, 0.013 * peri, True)
        if area >= 1e3:
            new = frame[y : h + y, x : x + w]
            new = cv2.cvtColor(new, cv2.COLOR_BGR2HSV)
            shapeC, predicS = shapeSolve(new, lower_color, upper_color)
            if shapeC == 1:
                if len(approx) == 3:
                    shape = "triangles"
                elif len(approx) == 4:
                    shape = "rectangles"
                elif len(approx) < 8:
                    shape = "pentagon"
                elif len(approx) == 8:
                    shape = "cross"
                else:
                    shape = "circle"
                if len(mission["m2"]) < 5:
                    mission["m2"].insert(i, shape)
                cv2.drawContours(frame, [approx], -1, (0, 255, 0), 3)
                cv2.putText(
                    frame,
                    f" {len(approx)} {shape}",
                    (x, y),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 100, 255),
                    2,
                )


def binaryDetection(Loop_x, Loop_y, frame):
    temp = []
    x, y, w, h = 5, 75, 50, 50
    for i in range(Loop_x):
        for j in range(Loop_y):
            new = frame[
                y + (j * h) : y + h + (j * h),
                x + (i * w) : x + w + (i * w),
            ]
            gray = cv2.cvtColor(new, cv2.COLOR_BGR2GRAY)
            binary, predicB = binarySolve(new, gray, 120)
            temp.append(binary)
            if len(temp) >= 3 and len(mission["m1"]) < 5:
                decimal_value = binaryList2Decimal(temp)
                mission["m1"].append(decimal_value)
            cv2.rectangle(
                frame,
                (x + (i * w), y + (j * h)),
                (x + w + (i * w), y + h + (j * h)),
                (0, 255, 0),
                2,
            )
            cv2.putText(
                frame,
                f" {binary} {round(predicB)}",
                (x + (i * w), y + (j * h)),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                (0, 100, 255),
                2,
            )


def shapeSolve(image, Lower, Upper):
    global mask_b
    predic = 0
    mask_b = cv2.inRange(image, Lower, Upper)
    mask_all = cv2.inRange(image, (0, 0, 0), (255, 255, 255))
    out_b = cv2.bitwise_and(image, image, mask=mask_b)
    out_all = cv2.bitwise_and(image, image, mask=mask_all)
    res_b = np.count_nonzero(out_b)
    res_all = np.count_nonzero(out_all)
    if res_all != 0:
        predic = (res_b / res_all) * 100
    if predic > 10:
        return 1, predic
    else:
        return 0, predic


def binarySolve(image, gray, Lower):
    predic = 0
    mask_b = cv2.inRange(gray, Lower, 255)
    mask_all = cv2.inRange(gray, 0, 255)
    out_b = cv2.bitwise_and(image, image, mask=mask_b)
    out_all = cv2.bitwise_and(image, image, mask=mask_all)
    res_b = np.count_nonzero(out_b)
    res_all = np.count_nonzero(out_all)
    if res_all != 0:
        predic = (res_b / res_all) * 100
    if predic >= 75:
        return 1, predic
    else:
        return 0, predic


def binaryList2Decimal(binary_list):
    decimal_value = 0
    power = len(binary_list) - 1

    for bit in binary_list:
        decimal_value += bit * (2**power)
        power -= 1

    return decimal_value


mission = {
    "m1": [],
    "m2": [],
    "m3": [],
}

while cap.isOpened():
    ret, frame = cap.read()
    # frame = cv2.flip(frame, 1)
    new_frame = frame.copy()

    scanMission(140, new_frame, True)

    cv2.imshow("Webcam", new_frame)

    if max_area != 0:
        warped = four_point_transform(new_frame, mission_contour.reshape(4, 2))
        new_warped = warped.copy()
        new_warped = cv2.resize(new_warped, (width, height))
        matrix(5, 3, new_warped)
        cv2.imshow("Mission", new_warped)
    print(mission)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
