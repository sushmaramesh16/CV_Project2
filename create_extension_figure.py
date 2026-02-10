import cv2
import numpy as np

# Read target
target = cv2.imread('data/olympus/pic.0535.jpg')

# Sobel texture matches (Task 4)
sobel_match1 = cv2.imread('data/olympus/pic.0171.jpg')
sobel_match2 = cv2.imread('data/olympus/pic.0454.jpg')
sobel_match3 = cv2.imread('data/olympus/pic.0629.jpg')

# Laws texture matches (Extension)
laws_match1 = cv2.imread('data/olympus/pic.0285.jpg')
laws_match2 = cv2.imread('data/olympus/pic.0628.jpg')
laws_match3 = cv2.imread('data/olympus/pic.0731.jpg')

# Resize all
size = (200, 200)
target = cv2.resize(target, size)
sobel_match1 = cv2.resize(sobel_match1, size)
sobel_match2 = cv2.resize(sobel_match2, size)
sobel_match3 = cv2.resize(sobel_match3, size)
laws_match1 = cv2.resize(laws_match1, size)
laws_match2 = cv2.resize(laws_match2, size)
laws_match3 = cv2.resize(laws_match3, size)

# Create layout:
# Row 1: Target (centered)
# Row 2: Sobel matches (3 images)
# Row 3: Laws matches (3 images)

spacer = np.zeros_like(target)

# Row 1: Spacer + Target + Spacer
row1 = np.hstack([spacer, target, spacer])

# Row 2: Sobel
row2 = np.hstack([sobel_match1, sobel_match2, sobel_match3])

# Row 3: Laws
row3 = np.hstack([laws_match1, laws_match2, laws_match3])

# Stack rows
grid = np.vstack([row1, row2, row3])

# Add labels
font = cv2.FONT_HERSHEY_SIMPLEX

# Target
cv2.putText(grid, 'Target: pic.0535.jpg', (220, 30), font, 0.7, (255, 255, 255), 2)

# Sobel section
cv2.putText(grid, 'Sobel Gradient Texture (Task 4):', (10, 220), font, 0.7, (255, 255, 0), 2)
cv2.putText(grid, 'pic.0171.jpg', (20, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.186', (20, 270), font, 0.4, (200, 200, 200), 1)
cv2.putText(grid, 'pic.0454.jpg', (220, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.189', (220, 270), font, 0.4, (200, 200, 200), 1)
cv2.putText(grid, 'pic.0629.jpg', (420, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.191', (420, 270), font, 0.4, (200, 200, 200), 1)

# Laws section
cv2.putText(grid, 'Laws Texture Filters (Extension):', (10, 420), font, 0.7, (0, 255, 0), 2)
cv2.putText(grid, 'pic.0285.jpg', (20, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.119', (20, 470), font, 0.4, (200, 200, 200), 1)
cv2.putText(grid, 'pic.0628.jpg', (220, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.121', (220, 470), font, 0.4, (200, 200, 200), 1)
cv2.putText(grid, 'pic.0731.jpg', (420, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'Dist: 0.136', (420, 470), font, 0.4, (200, 200, 200), 1)

# Add footer note
cv2.putText(grid, 'Note: Only pic.0628/0629 (nearly identical) appears in both', 
            (10, grid.shape[0] - 10), font, 0.5, (255, 0, 255), 1)

# Save
cv2.imwrite('results/extension_results.jpg', grid)
print("Saved to results/extension_results.jpg")

# Display
cv2.imshow('Extension Results - Sobel vs Laws', grid)
cv2.waitKey(0)
cv2.destroyAllWindows()