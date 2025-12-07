import FreeCAD, Part, math

# Rocko Dog Robot - FreeCAD Script for MG996R Servos
# Run this script inside FreeCAD's Python Console or Macro Editor

# --- Dimensions (mm) ---
# MG996R Servo Dimensions
servo_len = 40.7
servo_wid = 19.7
servo_dep = 37.0 # Mounting depth
mount_hole_dist = 48.0 # Distance between mounting holes (approx)

# Robot Parts
femur_len = 120.0 # 12cm
calf_len = 120.0 # 12cm
thickness = 6.0 # Thicker for strength
chassis_len = 240.0
chassis_wid = 140.0

doc = FreeCAD.newDocument("Rocko_MG996R")

# --- Helper Functions ---

def create_servo_cutout():
    # Main body hole
    box = Part.makeBox(servo_len, servo_wid, 50)
    box.translate(FreeCAD.Vector(-servo_len/2, -servo_wid/2, -25))
    
    # Mounting holes
    h1 = Part.makeCylinder(2, 50)
    h1.translate(FreeCAD.Vector(mount_hole_dist/2, 0, -25))
    h2 = Part.makeCylinder(2, 50)
    h2.translate(FreeCAD.Vector(-mount_hole_dist/2, 0, -25))
    
    return box.fuse([h1, h2])

def create_leg_link(length, name):
    # Main arm
    arm = Part.makeBox(length, 20, thickness)
    arm.translate(FreeCAD.Vector(0, -10, -thickness/2))
    
    # Rounded ends
    c1 = Part.makeCylinder(10, thickness)
    c1.translate(FreeCAD.Vector(0, 0, -thickness/2))
    
    c2 = Part.makeCylinder(8, thickness)
    c2.translate(FreeCAD.Vector(length, 0, -thickness/2))
    
    shape = arm.fuse([c1, c2])
    
    # Pivot Holes
    h1 = Part.makeCylinder(3, 50) # Servo horn screw
    h1.translate(FreeCAD.Vector(0, 0, -25))
    
    h2 = Part.makeCylinder(3, 50) # Knee/Foot pivot
    h2.translate(FreeCAD.Vector(length, 0, -25))
    
    final_shape = shape.cut([h1, h2])
    
    obj = doc.addObject("Part::Feature", name)
    obj.Shape = final_shape
    return obj

def create_chassis():
    # Main Plate
    plate = Part.makeBox(chassis_len, chassis_wid, thickness)
    plate.translate(FreeCAD.Vector(-chassis_len/2, -chassis_wid/2, 0))
    
    # Cutouts for Servos (4 corners)
    cutouts = []
    
    # Positions: FL, FR, BL, BR
    # Inset from corners
    inset_x = 30
    inset_y = 20
    
    positions = [
        (chassis_len/2 - inset_x, chassis_wid/2 - inset_y, 90),   # FL
        (chassis_len/2 - inset_x, -chassis_wid/2 + inset_y, -90), # FR
        (-chassis_len/2 + inset_x, chassis_wid/2 - inset_y, 90),  # BL
        (-chassis_len/2 + inset_x, -chassis_wid/2 + inset_y, -90) # BR
    ]
    
    for (x, y, rot) in positions:
        cutout = create_servo_cutout()
        cutout.rotate(FreeCAD.Vector(0,0,0), FreeCAD.Vector(0,0,1), rot)
        cutout.translate(FreeCAD.Vector(x, y, 0))
        cutouts.append(cutout)
        
    final_chassis = plate.cut(cutouts)
    
    # Weight reduction (Center hole)
    center_hole = Part.makeBox(chassis_len - 100, chassis_wid - 80, 50)
    center_hole.translate(FreeCAD.Vector(-(chassis_len-100)/2, -(chassis_wid-80)/2, -25))
    final_chassis = final_chassis.cut(center_hole)
    
    obj = doc.addObject("Part::Feature", "Chassis")
    obj.Shape = final_chassis
    return obj

# --- Generate Parts ---

create_chassis()
create_leg_link(femur_len, "Femur")
create_leg_link(calf_len, "Calf")

# Recompute to show
doc.recompute()
FreeCAD.Gui.SendMsgToActiveView("ViewFit")
