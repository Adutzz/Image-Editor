# Image-Editor
1) Main Function:
In the variable "command" I save the string input which tells me what
function I have to call and then enter a while loop which ends when the input
gives "command" the string "EXIT". If everything goes well, I free all the
remaining alocated memory with the function free_mem().

2) allocate_matrix()
This function is called after successfully loading an image to allocate
memory for the image matrix and populate it with pixel data.
Based on the image type (P1 to P6), it reads the pixel data from the file and stores it in the allocated matrix.
It handles both ASCII and binary formats.

3) save()
Saves the current image or selection to a file in ASCII or binary format.
The function checks whether the user wants to save the image in ASCII or binary format.
It writes the image data to the specified file, either as ASCII (for P1, P2, P3) or binary (for P4, P5, P6).

4) copy_dim()
Adjusts the selection dimensions, ensuring that x1 <= x2 and y1 <= y2.
It also computes the dimensions of the selection.
This function is used when creating a selection to ensure the coordinates
are correctly ordered.

5) sel()
It copies the selected region to a new matrix.
The function checks if the selection coordinates are in the correct order.
Memory for the selection is allocated, and the selected pixels are copied
to the new matrix. It frees the previous selection memory if any.

6) select_all()
Selects the entire image and allocates memory for the selection.
The function allocates memory for the selection, then copies the entire 
image into the new selection matrix.

7) crop()
The function frees the current image matrix, then allocates a new 
matrix with the dimensions of the selection. It copies the selected region 
into the new image matrix and updates the image size.

8) rotate()
The function performs a 90-degree clockwise rotation of the image 
or selection by rearranging the pixels in a new matrix. The rotation is 
performed by swapping rows and columns based on the rotation angle.
By rotating the matrix multiple times in the same direction, it gets all the
angles.
