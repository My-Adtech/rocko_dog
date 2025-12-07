// Rocko Dog Robot - 3D Printable Parts
// OpenSCAD Script

$fn = 50; // Resolution

// Servo Type: 0 = MG90S (Micro), 1 = MG996R (Standard)
servo_type = 0; 

// Dimensions based on servo type
servo_width = (servo_type == 0) ? 12.5 : 20;
servo_length = (servo_type == 0) ? 23 : 40.7;
servo_height = (servo_type == 0) ? 23 : 37; // Body height
hole_dia = (servo_type == 0) ? 2 : 3;

femur_len = (servo_type == 0) ? 100 : 120; // Longer legs for bigger servos
calf_len = (servo_type == 0) ? 100 : 120;
thickness = (servo_type == 0) ? 4 : 6; // Thicker parts for bigger servos

// --- Modules ---

module servo_mount() {
    difference() {
        cube([servo_length + 4, servo_width + 4, 10], center=true);
        cube([servo_length, servo_width, 20], center=true); // Servo hole
        // Mounting holes
        translate([servo_length/2 + 1, 0, 0]) cylinder(h=20, r=1, center=true);
        translate([-servo_length/2 - 1, 0, 0]) cylinder(h=20, r=1, center=true);
    }
}

module leg_segment(length) {
    difference() {
        union() {
            hull() {
                cylinder(h=thickness, r=8, center=true);
                translate([length, 0, 0]) cylinder(h=thickness, r=6, center=true);
            }
        }
        // Pivot holes
        cylinder(h=20, r=hole_dia, center=true);
        translate([length, 0, 0]) cylinder(h=20, r=hole_dia, center=true);
    }
}

module chassis() {
    width = 120; // 12cm
    length = 200; // 20cm
    
    difference() {
        cube([length, width, thickness], center=true);
        // Cutouts for weight reduction
        cube([length-40, width-40, 20], center=true);
    }
    
    // Servo Mounts (Hips)
    for (x = [-1, 1]) {
        for (y = [-1, 1]) {
            translate([x*(length/2 - 15), y*(width/2 + 10), 0]) 
                rotate([0, 0, 90])
                servo_mount();
        }
    }
}

// --- Render ---

// Uncomment one to render
// chassis();
// translate([0, 50, 0]) leg_segment(femur_len); // Femur
// translate([0, 80, 0]) leg_segment(calf_len); // Calf

// Assembly Visualization
chassis();
translate([85, 70, 0]) rotate([0, 90, 0]) leg_segment(femur_len);
translate([85, 70, -100]) rotate([0, 90, 0]) leg_segment(calf_len);
