import cv2


def main():
    # Open the webcam (index 0)
    cap = cv2.VideoCapture(2)

    if not cap.isOpened():
        print("Error: Could not open webcam.")
        return

    while True:
        # Read a frame from the webcam
        ret, frame = cap.read()

        if not ret:
            print("Error: Could not read frame.")
            break

        # Display the frame
        cv2.imshow("Webcam", frame)

        # Press 'q' to exit
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    # Release the webcam and close the window
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
