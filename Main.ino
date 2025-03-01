#include <math.h>
#include <UARTClass.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TwoWire Wire(1);  // I2C-1 (Adjust if using a different I2C port)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (not used)
#define SCREEN_ADDRESS 0x3C //  OLED Address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
UARTClass Serial1(1);

// Model configuration from your text files
#define HIDDEN_WEIGHTS_SIZE 288    // 3x3x1x32 (from hidden_weights.txt)
#define HIDDEN_BIASES_SIZE 32       // (from hidden_biases.txt)
#define OUTPUT_WEIGHTS_SIZE 64     // 32x2 (from output_weights.txt)
#define OUTPUT_BIASES_SIZE 2       // (from output_biases.txt)
#define INPUT_SIZE 9216



// Global arrays with correct dimensions from the Python model

float hidden_weights[HIDDEN_WEIGHTS_SIZE] = {
    // Content from hidden_weights.txt (first 288 values)
    0.148456350,0.090600014,0.098053262,0.014739756,-0.153062880,-0.054833159,0.116474070,0.034648411,0.143373370,0.057405166,-0.078556642,-0.023963206,0.019818764,-0.098183431,
-0.005797259,0.202940404,-0.118339978,0.135708362,-0.005339365,0.004725190,0.202668220,0.056532055,0.131358594,-0.089967392,0.076176539,0.042825043,-0.011263247,-0.028180907,
0.124121934,0.021436123,-0.033025790,0.101466380,0.054703459,0.104298703,-0.138085574,0.153331622,-0.043500185,-0.076985501,-0.052023008,-0.026866879,-0.001857288,-0.016101250,
0.081556812,-0.075614691,0.041057482,0.012884500,0.134872675,0.040523801,0.011219553,-0.110177271,0.122283079,-0.134468421,0.029308887,-0.031088086,-0.030572884,-0.088158913,
-0.083202973,0.091742605,-0.140198722,-0.106292143,-0.138660267,-0.113735542,-0.096882425,-0.126705945,-0.049772102,0.029182050,-0.078000188,0.005013481,0.106685065,0.093684234,
0.098131448,-0.076859131,0.152768537,-0.133694097,0.001021142,0.036006913,-0.068476409,0.103892162,0.135672972,-0.061545767,-0.091282479,-0.108067364,0.131343201,0.127262414,
0.022356596,0.063423842,-0.065700740,0.076325729,0.114921115,0.013546292,-0.118206166,0.015170383,0.083600871,0.190828308,0.206033289,0.104821034,0.097413518,0.063415751,-0.009731835,
0.075813361,0.089344807,-0.045113184,0.009854730,0.097956643,0.116800003,-0.095429018,-0.097608954,-0.081914052,0.123526633,-0.103148147,0.046436973,-0.032300163,-0.138769567,
0.048023872,0.001068467,0.053256460,0.001975094,0.022337016,0.094436392,-0.091172099,-0.009762502,0.137511790,-0.088126548,-0.071832642,0.138271704,0.004933960,-0.182866275,0.081041686,
0.086169206,-0.055289131,0.056654189,0.145517349,-0.076228447,0.002501676,-0.076617919,-0.047898505,-0.107555211,0.023360750,0.137917668,-0.094014451,0.069262311,0.123211190,
-0.064965419,-0.161464140,-0.129229382,0.119529866,-0.086108811,0.105544493,-0.082075402,-0.097860366,0.083518133,-0.035549246,-0.034016903,0.009025135,0.166310340,0.098660909,
0.126712218,0.066527218,-0.096314833,-0.156238049,-0.038575485,0.027062412,-0.058135644,-0.139389560,-0.156791940,0.060074486,0.108888462,0.124033548,0.015365385,-0.042827748,
0.068063758,0.024197156,0.016733728,0.057944082,-0.088220663,-0.198252246,0.153633624,0.109711342,-0.045599286,-0.019431747,-0.119724326,0.049514771,0.014730317,0.064776018,
-0.095346965,0.081471384,-0.097092740,0.094047815,-0.117450550,0.137500748,0.077026397,-0.159668356,0.074905366,-0.001342812,0.023069428,-0.098796226,0.108279102,-0.027474221,
0.047317266,-0.146995857,-0.128242508,0.072313361,-0.103943579,0.200297460,-0.010523385,-0.101135917,-0.079233840,0.038298044,0.059304647,0.026015418,0.079600431,0.058578875,
0.055583797,-0.016241105,0.141082421,0.042097244,0.107826516,-0.105340786,-0.097549543,-0.064097002,-0.004547698,-0.148873344,0.026432572,-0.085847005,-0.126310796,-0.149595052,
0.000119512,0.057895590,-0.018348230,0.032113593,0.116412230,0.006398035,-0.031414315,0.000053479,0.107712403,0.009394955,-0.008618656,-0.114509791,-0.092194453,0.110657945,
0.045338057,-0.038612690,-0.035668120,-0.028917005,0.009725081,0.120721199,-0.142161876,0.029584054,-0.031386469,-0.135018438,0.135174036,0.133885100,-0.123148978,-0.134453669,
-0.119079694,0.150643885,0.010182516,-0.101188503,0.115243353,-0.075180002,0.125715837,-0.002504217,0.037561849,0.068616480,-0.162455529,0.080939181,-0.064127252,-0.038492944,
0.071498126,0.013670680,-0.114457041,-0.191819265,0.152388647,-0.085532941,0.071427517,-0.152344018,-0.095397368,0.048249282,-0.038519196,0.111741610,0.113531835,-0.107650712,
0.134058192,0.053921875,0.022206580,0.054592066,-0.083653547,0.106407277
 /* ... 286 more values */ 
};


float hidden_biases[HIDDEN_BIASES_SIZE] = {
    // Content from hidden_biases.txt (32 values)
    -0.046988286,0.010635473,-0.003094428,-0.040166143,0.002212931,0.019426873,-0.047658611,-0.015985850,0.022378325,0.046017542,-0.017569650,0.030911356,-0.039501116,0.054882873,
0.063853942,0.065961331,0.033605866,-0.031478167,-0.044953197,-0.006193758,-0.004446193,-0.047835454,-0.044770122,-0.011885540,-0.046948142,-0.006338993,0.067086957,-0.040610075,
-0.037729979,-0.024251405,0.085523747,-0.001717911
 /* ... 30 more values */
};

float output_weights[OUTPUT_WEIGHTS_SIZE] = {
    // Content from output_weights.txt (64 values)
    -0.027378323,0.020304032,-0.001035254,-0.010952076,0.011254237,-0.018656289,-0.024114620,0.017677754,-0.076634452,0.080162264,-0.056619439,0.056344297,-0.058033634,0.052918196,
-0.009302006,0.010548403,0.019387178,-0.031049935,0.005659167,0.003112333,-0.045110978,0.044732086,-0.021774437,0.023483505,-0.024325190,0.022977125,-0.025009144,0.032544106,
0.018916408,-0.019492276,0.107537925,-0.110228039,-0.059403859,0.044402339,-0.022912581,0.022850277,-0.045140646,0.047788128,-0.069595128,0.063266516,0.044930678,-0.029874649,
-0.054727707,0.063891284,-0.034809310,0.036163777,-0.023206985,0.013681388,-0.043024089,0.046679150,0.005990026,-0.005307009,-0.039229643,0.043819889,-0.046276350,0.036932763,
-0.028933115,0.028420150,-0.029645538,0.041532356,-0.033200756,0.027993403,-0.005602550,0.008867283 /* ... 62 more values */
};

float output_biases[OUTPUT_BIASES_SIZE] = {
    // Content from output_biases.txt (2 values)
    -3.170993551611900330e-02, -1.112306583672761917e-02
};


//input in the form of array
float output[OUTPUT_BIASES_SIZE];

// Activation function (ReLU)
void relu(float* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = fmax(0, arr[i]);
    }
}

// Softmax function for output layer
void softmax(float* input, int length) {
    float max = input[0];
    float sum = 0.0;
    
    // Find max for numerical stability
    for(int i = 1; i < length; i++) {
        if(input[i] > max) max = input[i];
    }
    
    // Calculate exponentials and sum
    for(int i = 0; i < length; i++) {
        input[i] = exp(input[i] - max);
        sum += input[i];
    }
    
    // Normalize
    for(int i = 0; i < length; i++) {
        input[i] /= sum;
    }
}

// Forward pass function
void predict(float* input, float* output) {
    float hidden_layer[HIDDEN_BIASES_SIZE] = {0};
    for (int i = 0; i < HIDDEN_BIASES_SIZE; i++) {
        hidden_layer[i] = hidden_biases[i];
        for (int j = 0; j < (INPUT_SIZE / HIDDEN_BIASES_SIZE); j++) {
            int weight_index = i * (INPUT_SIZE / HIDDEN_BIASES_SIZE) + j;
            if (weight_index < HIDDEN_WEIGHTS_SIZE && j < INPUT_SIZE) {
                hidden_layer[i] += hidden_weights[weight_index] * input[j];
            }
        }
    }
    relu(hidden_layer, HIDDEN_BIASES_SIZE);
    
    for (int i = 0; i < OUTPUT_BIASES_SIZE; i++) {
        output[i] = output_biases[i];
        for (int j = 0; j < HIDDEN_BIASES_SIZE; j++) {
            int weight_index = i * HIDDEN_BIASES_SIZE + j;
            if (weight_index < OUTPUT_WEIGHTS_SIZE) {
                output[i] += output_weights[weight_index] * hidden_layer[j];
            }
        }
    }
    softmax(output, OUTPUT_BIASES_SIZE);
}

bool dataPrinted = false; 
bool datareceived = false;



int data[INPUT_SIZE];
float dataf[INPUT_SIZE];
int i=0;


void setup() {
    Serial.begin(115200);
     Serial1.begin(115200);
  
  // Wait a moment for UART to initialize
  delay(1000);

    
       if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial1.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

 // display.setCursor(0, 0);
 // display.println("* Naa Ready 123456 *");
  display.display();
}

void displayPredictionResults(const char* predicted_class, float max_prob, float* class_probabilities, const char* class_labels[]) {
  display.clearDisplay();
  display.setTextSize(1);
  
  // Title
  display.setCursor(0, 0);
  display.println("Prediction Results");
  display.drawLine(0, 9, display.width(), 9, SSD1306_WHITE);
  
  // Predicted Class
  display.setCursor(0, 12);
  display.print("Predicted Class: ");
  display.println(predicted_class);
  
  // Confidence
  display.setCursor(0, 22);
  display.print("Confidence: ");
  display.print(max_prob * 100, 1);
  display.println("%");
  
  // Class Probabilities
  display.setCursor(0, 34);
  display.println("Probabilities:");
  for(int i = 0; i < 2; i++) {
    display.setCursor(0, 44 + (i * 10));
    display.print(class_labels[i]);
    display.print(": ");
    display.print(class_probabilities[i] * 100, 1);
    display.println("%");
  }
  
  display.display();
}




void loop() {
    // Your loop code here
    if(i==0){
      
      display.setCursor(7,0);
      display.setTextSize(2);
      display.println("TbDetector");
      display.setTextSize(1);
      display.println(" ");
      display.println(" ");
      display.println("by: ");
      display.println("   Arivarasan VM");
      display.println("   Chandru M");
      display.println("  Team VLSI,SASTRA  ");
      display.display();
    }
   if(i <INPUT_SIZE+2){
    if (Serial.available() ) {
    char incomingChar = Serial.read();
    Serial1.print(incomingChar);
    byte value = (byte)incomingChar;
    if(i==1){
      display.clearDisplay();
      display.setCursor(15, 0);
      display.println(" ");
      display.println(" ");
      display.println(" ");
      display.println(" ");
      display.println("-- Receiving  data --");
      display.display();
    }
  if(i>1){
  // Print the received value
  Serial1.print("[");
  Serial1.print(i-2);
  Serial1.print("]=");
  Serial1.println(value);
  data[i-2]=value;
    }    
    i++;
    }
   }
    if (i==INPUT_SIZE+1) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" ");
      display.println(" ");
      display.println(" ");
      display.println(" ");
      display.println("-- done receiving --");
      display.println(" ");
      display.println(" ");
      display.println("     Analysing...   ");
      display.display();
      delay(1000);
     for(int k = 1; k < INPUT_SIZE; k++) {
         dataf[k] =(float)data[k] / 255.0;
     }
     datareceived = true;
     i++;
    }
    if(datareceived && !dataPrinted){
    predict(dataf, output);
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(" ");
      display.println(" ");
      display.println("-- done processing --");
      display.display();
      delay(1000);
    // Find the class with highest probability
    float max_prob = output[0];
    int predicted_class = 0;
 
        if(output[1] > max_prob) {
            max_prob = output[1];
            predicted_class = 1;
        }
    
    
    // Print results
    Serial.println("  ");
    Serial.println("Prediction Results:");
    Serial.println("  ");
    Serial.println("----------------------------");
    Serial.println("  ");
    const char* class_labels[] = {"Tuberculosis","Normal"};
    Serial.print("Predicted Class: ");
    Serial.println(class_labels[predicted_class]);
    Serial.println("  ");
    Serial.print("Confidence: ");
    Serial.print(max_prob * 100);
    Serial.println("%");
    Serial.println("  ");
    // Print all class probabilities
    for(int i = 0; i < 2; i++) {
        Serial.print(class_labels[i]);
        Serial.print(": ");
        Serial.print(output[i] * 100);
        Serial.println("%");
    }
    Serial.println("============================");

    delay(2000);
    display.clearDisplay();
      display.setCursor(0, 0);
      display.println("* done printing *");
      display.display();

displayPredictionResults(class_labels[predicted_class],max_prob,output,class_labels);
//displayPredictionResults(class_labels[predicted_class], max_prob * 100, output, class_labels);

    delay(200000);
    dataPrinted = true;
    i=0;
    }
      
}
