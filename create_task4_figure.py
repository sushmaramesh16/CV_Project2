import cv2
import numpy as np

# Read target
target = cv2.imread('data/olympus/pic.0535.jpg')

# Color-only (Task 2) matches
color_match1 = cv2.imread('data/olympus/pic.0285.jpg')
color_match2 = cv2.imread('data/olympus/pic.0628.jpg')
color_match3 = cv2.imread('data/olympus/pic.0952.jpg')

# Color+Texture (Task 4) matches
texture_match1 = cv2.imread('data/olympus/pic.0171.jpg')
texture_match2 = cv2.imread('data/olympus/pic.0454.jpg')
texture_match3 = cv2.imread('data/olympus/pic.0629.jpg')

# Resize all
size = (200, 200)
target = cv2.resize(target, size)
color_match1 = cv2.resize(color_match1, size)
color_match2 = cv2.resize(color_match2, size)
color_match3 = cv2.resize(color_match3, size)
texture_match1 = cv2.resize(texture_match1, size)
texture_match2 = cv2.resize(texture_match2, size)
texture_match3 = cv2.resize(texture_match3, size)

# Create layout:
# Row 1: Target (centered)
# Row 2: Color-only matches (3 images)
# Row 3: Color+Texture matches (3 images)

spacer = np.zeros_like(target)

# Row 1: Spacer + Target + Spacer
row1 = np.hstack([spacer, target, spacer])

# Row 2: Color-only
row2 = np.hstack([color_match1, color_match2, color_match3])

# Row 3: Color+Texture
row3 = np.hstack([texture_match1, texture_match2, texture_match3])

# Stack rows
grid = np.vstack([row1, row2, row3])

# Add labels
font = cv2.FONT_HERSHEY_SIMPLEX

# Target
cv2.putText(grid, 'Target: pic.0535.jpg', (220, 30), font, 0.7, (255, 255, 255), 2)

# Color-only section
cv2.putText(grid, 'Color-Only Histogram (Task 2):', (10, 220), font, 0.7, (0, 255, 255), 2)
cv2.putText(grid, 'pic.0285.jpg', (20, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0628.jpg', (220, 250), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0952.jpg', (420, 250), font, 0.5, (255, 255, 255), 1)

# Color+Texture section
cv2.putText(grid, 'Color + Texture (Task 4):', (10, 420), font, 0.7, (0, 255, 0), 2)
cv2.putText(grid, 'pic.0171.jpg', (20, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0454.jpg', (220, 450), font, 0.5, (255, 255, 255), 1)
cv2.putText(grid, 'pic.0629.jpg', (420, 450), font, 0.5, (255, 255, 255), 1)

# Add footer note
cv2.putText(grid, 'Note: Only pic.0628/0629 (nearly identical) appears in both', 
            (10, grid.shape[0] - 10), font, 0.5, (255, 0, 255), 1)

# Save
cv2.imwrite('results/task4_results.jpg', grid)
print("Saved to results/task4_results.jpg")

# Display
cv2.imshow('Task 4 Results - Color vs Color+Texture', grid)
cv2.waitKey(0)
cv2.destroyAllWindows()