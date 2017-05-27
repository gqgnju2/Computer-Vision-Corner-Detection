# Computer-Vision-Corner-Detection

Fast Corner Detection
FAST is an algorithm proposed originally by Rosten and Drummond [1] for identifying interest points in an image. The reason behind the work of the FAST algorithm was to develop an interest point detector for use in real time frame rate applications like SLAM on a mobile robot, which have limited computational resources [4]. 
2.1	Feature Detection using Fast
The Fast algorithm used to find the comers is presented below:
1.	Select a pixel p in the image. Assume the intensity of this pixel to be . This is the pixel which is to be identified as an interest point or not. 
2.	Set an appropriate threshold value t.
3.	Consider a Bresenham circle [4] of 16 pixels surrounding the pixel p.
4.	Now the pixel p is a corner if there exists a set of n contiguous pixels in the circle (of 16 pixels) which are all brighter than + t, or all darker than − t. n was chosen to be 12.
5.	To make the algorithm fast, a high-speed test was proposed to exclude a large number of non-corners. This test examines only the four pixels at 1, 9, 5 and 13 (First 1 and 9 are tested if they are too brighter or darker. If so, then checks 5 and 13). If p is a corner, then at least three of these must all be brighter than  + t or darker than − t. If neither of these is the case, then p cannot be a corner. The full segment test criterion can then be applied to the passed candidates by examining all pixels in the circle.
6.	Repeat the procedure for all the pixels in the image.



Figure 1: Image showing the interest point under test and the 16 pixels on the circle [1]


2.2	Non-maximal Suppression
Detection of multiple interest points adjacent to one another is one key problems of Fast algorithm. This can be dealt with by applying non maximal suppression after detecting the interest points. The algorithm is described below:

1.	Compute a score function V from each detected point. The definition of function V is the sum of the absolute difference between the pixels in circle and the centre pixel.
2.	Compare the V values of two adjacent interest points.
3.	Discard the one with lower V value.



