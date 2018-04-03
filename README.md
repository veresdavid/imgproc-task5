# ImgProc - Task 5

Developed in Visual Studio 2017.
Used dependency: OpenCV.

Very basic character recognition program.

Usage:
Training: ./program -t train_image.jpg train_output.txt
Recognition: ./program -r input_image.jpg database.txt output.txt

Implemented:
* threshold image
* segmentation of characters from images
* generate walsh matrices
* training (storing output in a text file)
* recognition (storing output in a text file)