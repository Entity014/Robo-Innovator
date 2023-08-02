import cv2
import requests
import numpy as np


def Detec(C, hsv, R, G, B, P):
    mask_r = cv2.inRange(hsv, R[0], R[1])
    mask_g = cv2.inRange(hsv, G[0], G[1])
    mask_b = cv2.inRange(hsv, B[0], B[1])
    mask_p = cv2.inRange(hsv, P[0], P[1])
    mask_all = cv2.inRange(hsv, (0, 0, 0), (255, 255, 255))
    out_r = cv2.bitwise_and(C, C, mask=mask_r)
    out_g = cv2.bitwise_and(C, C, mask=mask_g)
    out_b = cv2.bitwise_and(C, C, mask=mask_b)
    out_p = cv2.bitwise_and(C, C, mask=mask_p)
    out_all = cv2.bitwise_and(C, C, mask=mask_all)
    res_r = np.count_nonzero(out_r)
    res_g = np.count_nonzero(out_g)
    res_b = np.count_nonzero(out_b)
    res_p = np.count_nonzero(out_p)
    res_all = np.count_nonzero(out_all)
    predic_r = (res_r / res_all) * 100
    predic_g = (res_g / res_all) * 100
    predic_b = (res_b / res_all) * 100
    predic_p = (res_p / res_all) * 100
    # cv2.imshow("Test", C)
    if predic_g > 80:
        return 0
    elif predic_b > 80:
        return 1
    elif predic_p > 80:
        return 2
    elif predic_r > 80:
        return 3
    else:
        return "Error 404"


def Matrix(Loop_x, Loop_y):
    color_code = []
    for j in range(Loop_x):
        for i in range(Loop_y):
            new = img[y + (h * i) : y + h + (h * i), x + (w * j) : x + w + (w * j)]
            hsv_i = cv2.cvtColor(new, cv2.COLOR_BGR2HSV)
            b = Detec(new, hsv_i, Color_R, Color_G, Color_B, Color_P)
            color_code.append(b)
    print(color_code)
    return color_code


def Set_Station(S):
    a = Matrix(2, 2)
    STA = ""
    if a[1] == S[0][0] and a[2] == S[0][1] and a[3] == S[0][2]:
        STA = "Station1"
        # requests.get(Url[0])
    elif a[1] == S[1][0] and a[2] == S[1][1] and a[3] == S[1][2]:
        STA = "Station2"
        # requests.get(Url[1])
    elif a[1] == S[2][0] and a[2] == S[2][1] and a[3] == S[2][2]:
        STA = "Station3"
        # requests.get(Url[2])
    elif a[1] == S[3][0] and a[2] == S[3][1] and a[3] == S[3][2]:
        STA = "Station4"
        # requests.get(Url[3])
    else:
        STA = "Error 404"
    cv2.putText(img, str(STA), (x, y), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 0))


# img = cv2.imread('Station.png')
# img = cv2.resize(img, (520,620))

Url = [
    "http://127.0.0.1:1880/Detec_Barcode?Mydata_barcode=S1",
    "http://127.0.0.1:1880/Detec_Barcode?Mydata_barcode=S2",
    "http://127.0.0.1:1880/Detec_Barcode?Mydata_barcode=S3",
    "http://127.0.0.1:1880/Detec_Barcode?Mydata_barcode=S4",
]

Color_R = np.array([[9, 45, 115], [20, 255, 255]])
Color_G = np.array([[24, 20, 0], [101, 255, 255]])
Color_B = np.array([[102, 90, 0], [116, 255, 255]])
Color_P = np.array([[123, 35, 0], [149, 255, 235]])

Station = np.array([[1, 0, 2], [0, 1, 2], [0, 2, 1], [1, 2, 0]])

cam = cv2.VideoCapture(1)

while True:
    _, img = cam.read()
    h2, w2, qq = np.shape(img)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, Color_R[0], Color_R[1])
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for con in contours:
        rect = cv2.boundingRect(con)
        x, y, w, h = rect
        if (w * h) > 8000 and (x + w + w) < w2 and (y + h + h) < h2:
            cv2.rectangle(img, (x, y), (x + (w * 2), y + (h * 2)), (0, 255, 0), 1)
            # cv2.putText(img, str(w*h), (x, y), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 0))
            Set_Station(Station)

    cv2.imshow("mask", img)
    if cv2.waitKey(1) == ord("q"):
        break

img.release()
cv2.destroyAllWindows()
