import cv2
import numpy as np

mission = {
    "m0": {"index": [], "value": []},
    "m1": {"index": [], "value": []},
    "m2": {"index": [], "value": []},
}


def Detec(image, gray, Lower, Upper):
    _, mask_b = cv2.threshold(gray, Lower, Upper, cv2.THRESH_BINARY)
    _, mask_all = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY)
    out_b = cv2.bitwise_and(image, image, mask=mask_b)
    out_all = cv2.bitwise_and(image, image, mask=mask_all)
    res_b = np.count_nonzero(out_b)
    res_all = np.count_nonzero(out_all)
    predic_b = (res_b / res_all) * 100
    # print(predic_b)
    if predic_b > 80:
        return 1, predic_b
    else:
        return 0, predic_b


def detect_binary(image_path):
    image = cv2.imread(image_path)
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, thresholded = cv2.threshold(gray_image, 250, 255, cv2.THRESH_BINARY)
    blurred = cv2.GaussianBlur(thresholded, (5, 5), 0)
    edges = cv2.Canny(blurred, 50, 150)
    contours1, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    mission_index = 2
    temp_mis_ind = []
    temp_mis_val3 = []

    for index1, contour1 in enumerate(contours1):
        r = 0
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
            if len(temp_mis_ind) % 5 == 0:
                temp_mis_ind.reverse()
                mission[f"m{mission_index}"]["index"] = temp_mis_ind[:5]
                mission_index -= 1

        for index2, contour2 in enumerate(contours2):
            x, y, w, h = cv2.boundingRect(contour2)
            epsilon = 0.001 * cv2.arcLength(contour2, True)
            approx = cv2.approxPolyDP(contour2, epsilon, True)
            area = cv2.contourArea(contour2)
            num_vertices = len(approx)
            if num_vertices > 10:
                new_edges2 = new_edges1[y : h + y, x : x + w]
                new_image2 = new_image1[y : h + y, x : x + w]
                new_gray_image2 = new_gray_image1[y : h + y, x : x + w]
                binary, predic = Detec(new_image2, new_gray_image2, 200, 255)
                # x, y = approx[0][0]
                cv2.drawContours(new_image1, [approx], 0, (0, 255, 0), 2)
                cv2.putText(
                    new_image1,
                    f" {binary}",
                    (x, y),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.5,
                    (0, 100, 255),
                    2,
                )
                # cv2.imshow(f"Color{index1} {index2}", new_image1)

    cv2.imshow("Og image", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    image_path = "D:\Programming\Python\AI\RoboInnovator\Missio-768x547.png"
    lower_red = np.array([0, 162, 133])
    upper_red = np.array([179, 176, 169])
    detect_binary(image_path)
    # detect_polygon(image_path, lower_red, upper_red)
