import os
from PIL import Image, ImageDraw

def create_grass():
    # Grass texture: Green background with noise/variation
    img = Image.new('RGB', (256, 256), color=(34, 139, 34)) # ForestGreen
    draw = ImageDraw.Draw(img)
    import random
    random.seed(42)
    # Add noise and blades
    for _ in range(5000):
        x = random.randint(0, 255)
        y = random.randint(0, 255)
        g = random.randint(100, 200)
        r = random.randint(20, 80)
        b = random.randint(20, 60)
        draw.point((x, y), fill=(r, g, b))
    # Draw some grass blades
    for _ in range(500):
        x = random.randint(0, 255)
        y = random.randint(0, 255)
        length = random.randint(5, 12)
        angle = random.uniform(-0.2, 0.2)
        g = random.randint(140, 220)
        r = random.randint(30, 90)
        b = random.randint(30, 70)
        draw.line([x, y, x + int(length * angle), y - length], fill=(r, g, b), width=1)
    img.save('textures/grass.bmp')

def create_brick():
    # Brick texture: Red bricks with gray mortar
    img = Image.new('RGB', (256, 256), color=(165, 42, 42)) # Brown/Red
    draw = ImageDraw.Draw(img)
    import random
    random.seed(42)
    
    # Add noise to bricks for realism
    for x in range(256):
        for y in range(256):
            r, g, b = img.getpixel((x, y))
            noise = random.randint(-15, 15)
            img.putpixel((x, y), (max(0, min(255, r + noise)), 
                                   max(0, min(255, g + noise)), 
                                   max(0, min(255, b + noise))))
            
    # Draw mortar lines
    mortar_color = (180, 180, 180)
    brick_h = 32
    # Horizontal lines
    for y in range(0, 256, brick_h):
        draw.line([0, y, 256, y], fill=mortar_color, width=2)
        
    # Vertical lines (offset every row)
    for i, y in enumerate(range(0, 256, brick_h)):
        offset = 0 if i % 2 == 0 else 64
        for x in range(offset, 256 + 64, 128):
            draw.line([x, y, x, y + brick_h], fill=mortar_color, width=2)
            
    img.save('textures/brick.bmp')

def create_tiles():
    # Roof tiles: Gray/Plomo curved tiles
    img = Image.new('RGB', (256, 256), color=(105, 105, 105)) # DimGray
    draw = ImageDraw.Draw(img)
    import random
    random.seed(42)
    
    # Add noise
    for x in range(256):
        for y in range(256):
            r, g, b = img.getpixel((x, y))
            noise = random.randint(-10, 10)
            img.putpixel((x, y), (max(0, min(255, r + noise)), 
                                   max(0, min(255, g + noise)), 
                                   max(0, min(255, b + noise))))
            
    tile_w = 32
    tile_h = 64
    # Draw overlapping U-shape tile patterns
    for y in range(0, 256, tile_h):
        for x in range(0, 256, tile_w):
            # Highlight border
            draw.arc([x, y, x + tile_w, y + tile_h], 0, 180, fill=(140, 140, 140), width=2)
            # Shadow border
            draw.line([x, y + tile_h//2, x, y + tile_h], fill=(50, 50, 50), width=2)
            draw.line([x + tile_w, y + tile_h//2, x + tile_w, y + tile_h], fill=(50, 50, 50), width=2)
            
    img.save('textures/tiles.bmp')

def create_wood():
    # Wood texture: Brown with grain lines
    img = Image.new('RGB', (256, 256), color=(139, 90, 43)) # Wood brown
    draw = ImageDraw.Draw(img)
    import random
    import math
    random.seed(42)
    
    # Wavy wood grain lines
    for y in range(256):
        for x in range(256):
            # Apply a sine wave to create wood rings/grain
            val = math.sin(x * 0.05 + math.sin(y * 0.02) * 5)
            # Add noise
            r_val = random.uniform(0.85, 1.15)
            if val > 0.8:
                c = (int(90 * r_val), int(55 * r_val), int(25 * r_val))
            elif val > 0.4:
                c = (int(115 * r_val), int(75 * r_val), int(35 * r_val))
            else:
                c = (int(139 * r_val), int(90 * r_val), int(43 * r_val))
            img.putpixel((x, y), c)
            
    img.save('textures/wood.bmp')

def create_leaves():
    # Leaves texture: Dense foliage green
    img = Image.new('RGB', (256, 256), color=(34, 100, 34))
    draw = ImageDraw.Draw(img)
    import random
    random.seed(42)
    
    # Draw many leaf shapes overlapping
    for _ in range(800):
        cx = random.randint(0, 255)
        cy = random.randint(0, 255)
        rx = random.randint(5, 15)
        ry = random.randint(3, 8)
        
        # Color variation
        g = random.randint(80, 160)
        r = random.randint(20, g - 40)
        b = random.randint(20, 40)
        
        # Draw ellipse representing leaf
        draw.ellipse([cx - rx, cy - ry, cx + rx, cy + ry], fill=(r, g, b))
        # Leaf vein
        draw.line([cx - rx, cy, cx + rx, cy], fill=(r//2, g//2, b//2), width=1)
        
    img.save('textures/leaves.bmp')

if __name__ == '__main__':
    os.makedirs('textures', exist_ok=True)
    create_grass()
    create_brick()
    create_tiles()
    create_wood()
    create_leaves()
    print("All textures created successfully in lab_09/textures/")
