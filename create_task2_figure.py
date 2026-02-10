import cv2
import numpy as np

# Read target image
target = cv2.imread('data/olympus/pic.0164.jpg')

# RGB Histogram matches
rgb_match1 = cv2.imread('data/olympus/pic.0110.jpg')
rgb_match2 = cv2.imread('data/olympus/pic.1032.jpg')
rgb_match3 = cv2.imread('data/olympus/pic.0092.jpg')

# HSV Histogram matches
hsv_match1 = cv2.imread('data/olympus/pic.0080.jpg')
hsv_match2 = cv2.imread('data/olympus/pic.0599.jpg')
hsv_match3 = cv2.imread('data/olympus/pic.0898.jpg')

# Resize all to same size
size = (200, 200)
target = cv2.resize(target, size)
rgb_match1 = cv2.resize(rgb_match1, size)
rgb_match2 = cv2.resize(rgb_match2, size)
rgb_match3 = cv2.resize(rgb_match3, size)
hsv_match1 = cv2.resize(hsv_match1, size)
hsv_match2 = cv2.resize(hsv_match2, size)
hsv_match3 = cv2.resize(hsv_match3, size)

# Create layout: 
# Row 1: Target (centered) with labels
# Row 2: RGB matches (3 images)
# Row 3: HSV matches (3 images)

# Create spacer (black image)
spacer = np.zeros_like(target)

# Row 1: Spacer + Target + Spacer (to center the target)
row1 = np.hstack([spacer, target, spacer])

# Row 2: RGB matches
row2 = np.hstack([rgb_match1, rgb_match2, rgb_match3])

# Row 3: HSV matches
row3 = np.hstack([hsv_match1, hsv_match2, hsv_match3])

# Stack all rows
grid = np.vstack([row1, row2, row3])

# Add labels
font = cv2.FONT_HERSHEY_SIMPLEX

# Target label
cv2.putText(grid, 'Target: pic.0164.jpg', (220, 30), font, 0.6, (255, 255, 255), 2)

# RGB section label (above row 2)
cv2.putText(grid, 'RGB Histogram Matching:', (10, 220), font, 0.7, (0, 255, 255), 2)
cv2.putText(grid, 'pic.0110.jpg', (20, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.1032.jpg', (220, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0092.jpg', (420, 250), font, 0.5, (255, 255, 255), 1)

# HSV section label (above row 3)
cv2.putText(grid, 'HSV Histogram Matching:', (10, 420), font, 0.7, (0, 255, 0), 2)
cv2.putText(grid, 'pic.0080.jpg', (20, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0599.jpg', (220, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0898.jpg', (420, 450), font, 0.5, (255, 255, 255), 1)

# Save
cv2.imwrite('results/task2_results.jpg', grid)
print("Saved to results/task2_results.jpg")

# Display
cv2.imshow('Task 2 Results - RGB vs HSV', grid)
cv2.waitKey(0)
cv2.destroyAllWindows()