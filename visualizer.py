import serial
import threading
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class MPUReader:
    def __init__(self, port="/dev/ttyACM0", baudrate=115200):
        self.ser = serial.Serial(
            port,
            baudrate,
            timeout=1
        )

        self.roll = 0.0
        self.pitch = 0.0
        self.yaw = 0.0

        self.running = True

        self.thread = threading.Thread(
            target=self.read_loop,
            daemon=True
        )

        self.thread.start()

    def read_loop(self):
        while self.running:
            line = self.ser.readline().decode(
                "utf-8",
                errors="ignore"
            )

            values = read_line(line)

            if values is not None:
                self.roll, self.pitch, self.yaw = values

    def stop(self):
        self.running = False
        self.ser.close()
 

def read_line(line):
    try:
        roll, pitch, yaw = map(float, line.strip().split(","))

        return roll, pitch, yaw

    except ValueError:
        return None

def main():
    mpu = MPUReader(
        port="/dev/ttyACM0",
        baudrate=115200
    )
    print("Connected to MPU 6050")

    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlim([-1.5, 1.5])
    ax.set_ylim([-1.5, 1.5])
    ax.set_zlim([-1.5, 1.5])
    ax.set_xlabel("X (Roll Axis)")
    ax.set_ylabel("Y (Pitch Axis)")
    ax.set_zlabel("Z (Yaw Axis)")
    ax.set_title("Roll/Pitch/Yaw Visualization")

    origin = np.array([0, 0, 0])
    x_axis = np.array([1, 0, 0])
    y_axis = np.array([0, 1, 0])
    z_axis = np.array([0, 0, 1])

    qx = ax.quiver(*origin, *x_axis, color='red', linewidth=3, label='Roll (X)')
    qy = ax.quiver(*origin, *y_axis, color='green', linewidth=3, label='Pitch (Y)')
    qz = ax.quiver(*origin, *z_axis, color='blue', linewidth=3, label='Yaw (Z)')

    ax.legend(loc='upper left')

    def rotation_matrix(roll_deg, pitch_deg, yaw_deg):
        roll = np.radians(roll_deg)
        pitch = np.radians(pitch_deg)
        yaw = np.radians(yaw_deg)

        Rx = np.array([[1, 0, 0], 
                       [0, np.cos(roll), -np.sin(roll)], 
                       [0, np.sin(roll), np.cos(roll)]])

        Ry = np.array([[np.cos(pitch), 0, np.sin(pitch)], 
                       [0, 1, 0], 
                       [-np.sin(pitch), 0, np.cos(pitch)]])

        Rz = np.array([[np.cos(yaw), -np.sin(yaw), 0], 
                       [np.sin(yaw), np.cos(yaw), 0], 
                       [0, 0, 1]])

        return Rz @ Ry @ Rx

    try:
        while plt.get_fignums():
            roll = mpu.roll
            pitch = mpu.pitch
            yaw = mpu.yaw

            R = rotation_matrix(roll, pitch, yaw)

            new_x = R @ x_axis
            new_y = R @ y_axis
            new_z = R @ z_axis

            ax.cla()
            ax.set_xlim([-1.5, 1.5])
            ax.set_ylim([-1.5, 1.5])
            ax.set_zlim([-1.5, 1.5])

            ax.set_xlabel("X")
            ax.set_ylabel("Y")
            ax.set_zlabel("Z")

            ax.set_title(f"Roll: {roll:.1f} Pitch: {pitch:.1f} Yaw: {yaw:.1f}")

            ax.quiver(0, 0, 0, new_x[0], new_x[1], new_x[2], color='red', linewidth=3, label='Roll (X)')
            ax.quiver(0, 0, 0, new_y[0], new_y[1], new_y[2], color='green', linewidth=3, label='Pitch (Y)')
            ax.quiver(0, 0, 0, new_z[0], new_z[1], new_z[2], color='blue', linewidth=3, label='Yaw (Z)')

            ax.legend(loc='upper left')

            ax.view_init(elev=30, azim=60)

            plt.draw()
            plt.pause(0.05)

    except KeyboardInterrupt:
        print("\nStopping...")

    finally:
        mpu.stop()
        print("Disconnected.")


if __name__ == "__main__":
    main()