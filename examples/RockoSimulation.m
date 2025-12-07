% Rocko Dog Robot - Quadruped Trot Simulation
% Simulates the logic of the Arduino code (Trot Gait + IK) for 4 legs

clear; clc; close all;

%% Robot Dimensions (cm) - Updated for MG996R Servos
femur = 12;
calf = 12;
body_length = 24;
body_width = 14;

%% Gait Parameters (Trot)
% Trot: Diagonals move together.
% Pair 1: FL (Leg 0) & BR (Leg 3)
% Pair 2: FR (Leg 1) & BL (Leg 2)

num_steps = 40;
step_len = 4;
lift_height = 3;
stand_height = 11;

% Generate Swing and Stance trajectories
t = linspace(0, pi, num_steps);
swing_x = (step_len/2) * cos(t); % Forward motion (starts positive, goes negative? No, swing goes forward)
% Actually, swing moves FROM back TO front.
% Let's define x relative to hip center.
% Stance: +step/2 -> -step/2
% Swing: -step/2 -> +step/2

x_swing = linspace(-step_len/2, step_len/2, num_steps);
z_swing = stand_height - (lift_height * sin(linspace(0, pi, num_steps)));

x_stance = linspace(step_len/2, -step_len/2, num_steps);
z_stance = ones(1, num_steps) * stand_height;

% Full Cycle = Swing + Stance
cycle_x = [x_swing, x_stance];
cycle_z = [z_swing, z_stance];
total_points = length(cycle_x);

%% Simulation Loop
figure;
axis equal;
grid on;
view(3); % 3D View
xlabel('X (Forward)'); ylabel('Y (Left)'); zlabel('Z (Up)');
xlim([-20, 20]); ylim([-15, 15]); zlim([-15, 5]);
hold on;

% Hip Offsets (Body Frame)
% FL, FR, BL, BR
hip_offsets_x = [body_length/2, body_length/2, -body_length/2, -body_length/2];
hip_offsets_y = [body_width/2, -body_width/2, body_width/2, -body_width/2];

% Phase Offsets for Trot
% Legs 0 & 3 start at index 1 (Swing)
% Legs 1 & 2 start at index 41 (Stance)
indices = [1, 41, 41, 1]; 

for t = 1:total_points
    cla;
    
    % Draw Body
    % Simple rectangle connecting hips
    patch([body_length/2, body_length/2, -body_length/2, -body_length/2], ...
          [body_width/2, -body_width/2, -body_width/2, body_width/2], ...
          [0, 0, 0, 0], 'k', 'FaceAlpha', 0.1);
    
    for leg = 1:4
        % Get current index in cycle for this leg
        idx = mod(indices(leg) + t - 1, total_points);
        if idx == 0, idx = total_points; end
        
        local_x = cycle_x(idx);
        local_z = cycle_z(idx);
        
        % --- Inverse Kinematics (2D) ---
        L = sqrt(local_x^2 + local_z^2);
        theta = atan2(local_x, local_z);
        phi = acos((femur^2 + L^2 - calf^2) / (2 * femur * L));
        
        alpha = theta + phi; % Hip Angle
        % beta = ... (Knee angle, not needed for FK plotting)
        
        % --- Forward Kinematics for Plotting ---
        % Knee Pos (Local)
        k_x = femur * sin(alpha);
        k_z = femur * cos(alpha);
        
        % Foot Pos (Local) - Should match target
        f_x = local_x;
        f_z = local_z;
        
        % Transform to Global Frame
        % Global X = Hip X + Local X
        % Global Y = Hip Y
        % Global Z = Hip Z - Local Z (since Z is down in IK but Up in plot)
        
        hip_x = hip_offsets_x(leg);
        hip_y = hip_offsets_y(leg);
        hip_z = 0;
        
        global_knee_x = hip_x + k_x;
        global_knee_y = hip_y;
        global_knee_z = hip_z - k_z;
        
        global_foot_x = hip_x + f_x;
        global_foot_y = hip_y;
        global_foot_z = hip_z - f_z;
        
        % Draw Leg
        plot3([hip_x, global_knee_x], [hip_y, global_knee_y], [hip_z, global_knee_z], 'b-', 'LineWidth', 2);
        plot3([global_knee_x, global_foot_x], [global_knee_y, global_foot_y], [global_knee_z, global_foot_z], 'r-', 'LineWidth', 2);
        plot3(global_foot_x, global_foot_y, global_foot_z, 'go', 'MarkerSize', 4, 'MarkerFaceColor', 'g');
    end
    
    title('Rocko Dog - Trot Simulation');
    drawnow;
    pause(0.05);
end
