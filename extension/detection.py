import cv2
import numpy as np
import pytesseract

width, height = 800, 600


def detectBinary(image, gray, Lower, Upper):
    predic = 0
    mask_b = cv2.inRange(gray, Lower, Upper)
    mask_all = cv2.inRange(gray, 0, 255)
    out_b = cv2.bitwise_and(image, image, mask=mask_b)
    out_all = cv2.bitwise_and(image, image, mask=mask_all)
    res_b = np.count_nonzero(out_b)
    res_all = np.count_nonzero(out_all)
    if res_all != 0:
        predic = (res_b / res_all) * 100
    if predic > 80:
        return 1, predic
    else:
        return 0, predic


def detectShape(image, Lower, Upper):
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


def binaryList2Decimal(binary_list):
    decimal_value = 0
    power = len(binary_list) - 1

    for bit in binary_list:
        decimal_value += bit * (2**power)
        power -= 1

    return decimal_value


def robotVision(image):
    global mission
    mission = {
        "m1": {"index": [], "value": []},
        "m2": {"index": [], "value": []},
        "m3": {"index": [], "value": []},
    }

    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, thresholded = cv2.threshold(gray_image, 250, 255, cv2.THRESH_BINARY)
    blurred = cv2.GaussianBlur(thresholded, (5, 5), 0)
    edges = cv2.Canny(blurred, 50, 150)
    contours1, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    mission_index = 3
    temp_mis_ind = []
    shape = ""

    for index1, contour1 in enumerate(contours1):
        temp_mis_val3 = []
        x, y, w, h = cv2.boundingRect(contour1)
        new_edges1 = edges[y : h + y, x : x + w]
        new_image1 = image[y : h + y, x : x + w]
        new_gray_image1 = gray_image[y : h + y, x : x + w]
        new_edges1[0, :] = 0
        new_edges1[-1, :] = 0
        new_edges1[:, 0] = 0
        new_edges1[:, -1] = 0
        contours2, _ = cv2.findContours(
            new_edges1, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        if len(new_edges1) > 2:
            temp_mis_ind.append(index1)
            if len(temp_mis_ind) % 5 == 0 and mission_index >= 1:
                temp_mis_ind.reverse()
                mission[f"m{mission_index}"]["index"] = temp_mis_ind.copy()
                temp_mis_ind.clear()
                mission_index -= 1

        for index2, contour2 in enumerate(contours2):
            x, y, w, h = cv2.boundingRect(contour2)
            epsilon_filter = 0.001 * cv2.arcLength(contour2, True)
            approx_filter = cv2.approxPolyDP(contour2, epsilon_filter, True)
            epsilon_shape = 0.015 * cv2.arcLength(contour2, True)
            approx_shape = cv2.approxPolyDP(contour2, epsilon_shape, True)
            area = cv2.contourArea(contour2)
            num_filter = len(approx_filter)
            num_shape = len(approx_shape)
            if num_filter > 10:
                new_image2 = new_image1[y : h + y, x : x + w]
                new_gray_image2 = new_gray_image1[y : h + y, x : x + w]
                if len(mission[f"m3"]["value"]) < 5:
                    custom_config = r"--oem 3 --psm 6 outputbase digits"
                    extracted_text = pytesseract.image_to_string(
                        gray_image, config=custom_config
                    )
                    numbers_list = [int(num) for num in extracted_text if num.isdigit()]
                    mission[f"m3"]["value"] = numbers_list
                elif len(mission[f"m2"]["value"]) < 5:
                    new_image2 = cv2.cvtColor(new_image2, cv2.COLOR_BGR2HSV)
                    color, predicS = detectShape(new_image2, lower_color, upper_color)
                    if color == 1:
                        if num_shape == 3:
                            shape = "triangles"
                        elif num_shape == 4:
                            shape = "rectangles"
                        elif num_shape == 5:
                            shape = "pentagon"
                        elif num_shape > 5 and num_shape <= 10:
                            shape = "circle"
                        else:
                            shape = "cross"
                        mission[f"m2"]["value"].insert(0, shape)
                        cv2.drawContours(new_image1, [approx_filter], 0, (0, 255, 0), 2)
                        cv2.putText(
                            new_image1,
                            f" {shape}",
                            (x, y),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            0.5,
                            (0, 100, 255),
                            2,
                        )
                elif len(mission[f"m1"]["value"]) < 5:
                    binary, predicB = detectBinary(
                        new_image2, new_gray_image2, 210, 255
                    )
                    temp_mis_val3.append(binary)
                    if len(temp_mis_val3) >= 3:
                        temp_mis_val3.reverse()
                        decimal_value = binaryList2Decimal(temp_mis_val3)
                        mission[f"m1"]["value"].insert(0, decimal_value)
                    cv2.drawContours(new_image1, [approx_filter], 0, (0, 255, 0), 2)
                    cv2.putText(
                        new_image1,
                        f" {binary}",
                        (x, y),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        0.5,
                        (0, 100, 255),
                        2,
                    )

    # cv2.imshow("Og image", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    image_path = "D:\Programming\Python\AI\RoboInnovator\Missio-768x547.png"
    lower_color = np.array([0, 162, 133])
    upper_color = np.array([179, 176, 169])
    frame = cv2.imread(image_path)
    frame = cv2.resize(frame, (width, height))
    robotVision(frame)
    print(mission)
