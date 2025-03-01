import tkinter as tk
from tkinter import filedialog, scrolledtext, ttk
import cv2
import numpy as np
import serial
import time
from PIL import Image, ImageTk


class ImageAnalyzerGUI:
    def __init__(self, master):
        self.master = master
        master.title("X-Ray Image Analyzer")
        master.geometry("900x650")  # Increased height to accommodate new text lines
        master.configure(bg="#2C3E50")
        master.resizable(False, False)  # Prevent window resizing

        self.style = ttk.Style()
        self.style.theme_use('clam')
        self.style.configure('TButton', font=('Arial', 12), background='#3498DB', foreground='white')
        self.style.map('TButton', background=[('active', '#2980B9')])

        self.create_widgets()

    def create_widgets(self):
        # Title
        title = tk.Label(self.master, text="X-Ray Image Analyzer", font=("Arial", 24, "bold"), bg="#2C3E50", fg="white")
        title.pack(pady=20)

        # Main content frame
        content_frame = tk.Frame(self.master, bg="#2C3E50")
        content_frame.pack(expand=True, fill=tk.BOTH, padx=20, pady=20)

        # Frame for image and upload button
        #self.image_frame = tk.Frame(content_frame, bg="#2C3E50", width=400)
        #self.image_frame.pack(side=tk.LEFT, padx=(0, 10), fill=tk.Y)
        #self.image_frame.pack_propagate(False)  # Prevent frame from shrinking

        #self.image_label = tk.Label(self.image_frame, text="No image uploaded", bg="#34495E", fg="white", width=40, height=15)
        #self.image_label.pack(pady=(0, 10))
        self.upload_frame = tk.Frame(content_frame, bg="#2C3E50")
        self.upload_frame.pack(side=tk.LEFT, fill=tk.X, pady=(0, 20))

        self.upload_button = ttk.Button(self.upload_frame, text="Upload X-Ray Image", command=self.upload_image)
        self.upload_button.pack(expand=True)

        # Frame for results
        self.result_frame = tk.Frame(content_frame, bg="#2C3E50", width=400)
        self.result_frame.pack(side=tk.RIGHT, padx=(10, 0), fill=tk.BOTH, expand=True)
        self.result_frame.pack_propagate(False)  # Prevent frame from shrinking

        self.result_label = tk.Label(self.result_frame, text="Analysis Results", font=("Arial", 16, "bold"), bg="#2C3E50", fg="white")
        self.result_label.pack(pady=(0, 10))

        # Reduced size of result box
        self.result_text = scrolledtext.ScrolledText(self.result_frame, wrap=tk.WORD, width=45, height=15, bg="#ECF0F1", font=("Courier", 10))
        self.result_text.pack(expand=True, fill=tk.BOTH)
        self.result_text.tag_configure("justified", justify='center')
        self.result_text.tag_add("justified", "1.0", "end")

        # Status label
        self.status_label = tk.Label(self.master, text="Ready", bg="#2C3E50", fg="white", font=("Arial", 10))
        self.status_label.pack(side=tk.BOTTOM, pady=5)

        # Two new lines of text at the bottom
        self.bottom_text1 = tk.Label(self.master, text="TATA Electronics Supported VLSI Laboratory,SASTRA Deemed to be University", bg="#2C3E50", fg="white", font=("Arial", 10))
        self.bottom_text1.pack(side=tk.BOTTOM, pady=(0, 2))

        self.bottom_text2 = tk.Label(self.master, text="Team VLSI", bg="#2C3E50", fg="white", font=("Arial", 10))
        self.bottom_text2.pack(side=tk.BOTTOM, pady=(0, 2))

        self.bottom_text3 = tk.Label(self.master, text="by: Arivarasan VM & Chandru M", bg="#2C3E50", fg="white", font=("Arial", 10))
        self.bottom_text3.pack(side=tk.BOTTOM, pady=(0, 2))


    # ... (rest of the methods remain the same)
    def upload_image(self):
        file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.png *.jpg *.jpeg *.gif *.bmp")])
        if file_path:
            self.display_image(file_path)
            self.process_image(file_path)

    def display_image(self, file_path):
        image = Image.open(file_path)
        image = image.resize((300, 300), Image.LANCZOS)
        photo = ImageTk.PhotoImage(image)
        self.image_label.config(image=photo)
        self.image_label.image = photo

    def process_image(self, file_path):
        self.status_label.config(text="Processing image...")
        self.master.update()

        image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
        resized_image = cv2.resize(image, (96,96))
        #normalized_image = resized_image / 255.0
        img_flat=resized_image.flatten()
        img_flat=img_flat[:9216]
        #input_array = normalized_image.flatten().astype(np.float32)
        reshaped_array = img_flat[:9216].reshape(1,9216)
        float_array = reshaped_array.flatten().tolist()

        self.send_data_to_serial(float_array)

    def send_data_to_serial(self, float_array):
        self.status_label.config(text="Sending data to analyzer...")
        self.master.update()

        try:
            ser = serial.Serial('COM7',115200,timeout=1)
            time.sleep(2)

            while ser.in_waiting:
                print(ser.readline().decode().strip())
            print("Sending start marker...")
    #ser.write(b"Start\n")
            time.sleep(0.1)
            length_1=len(float_array)
            print(f"{length_1}")
            

            
            ser.write(b's')
            time.sleep(2)
            
            print("Sending float array...")
            for value in float_array:
                char_value = chr(value)
               # Send the character over serial
                ser.write(char_value.encode('latin-1'))
                print(f"Sent: {value} as '{char_value}' (hex: {hex(value)})") 
                #time.sleep(0.001)
                
            print("Sending end marker...")
            print("All data sent. Waiting for response...")

            self.receive_data_from_serial(ser)

        except serial.SerialException:
            self.status_label.config(text="Error: Could not connect to COM7")
        finally:
            if 'ser' in locals():
                ser.close()

    def receive_data_from_serial(self, ser):
        self.status_label.config(text="Receiving analysis results...")
        self.master.update()

        self.result_text.config(state=tk.NORMAL)
        self.result_text.delete(1.0, tk.END)

        while True:
            if ser.in_waiting:
                received_data = ser.readline().decode().strip()
                self.result_text.insert(tk.END, received_data + "\n", "justified")
                self.result_text.see(tk.END)
                self.master.update()

                if received_data == "============================":
                    break

        self.result_text.config(state=tk.DISABLED)
        self.status_label.config(text="Analysis complete")

root = tk.Tk()
gui = ImageAnalyzerGUI(root)
root.mainloop()
