#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t queue_1;

#define trigPin 3    // chân trig của HC-SR04
#define echoPin 4    // chân echo của HC-SR04
void setup() 
{
  Serial.begin(9600);   //giao tiếp Serial với baudrate 9600
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (13, OUTPUT);  //chân 13 phát tín hiệu đèn
  pinMode (12, OUTPUT);  //chân 12 phát tín hiệu còi 
  
  queue_1 = xQueueCreate(5, sizeof(int));
  if (queue_1 == NULL)
  {
  Serial.println("Queue can not be created!");
  }
  xTaskCreate(Taskdisplay, "Display_task", 128, NULL, 2, NULL);
  xTaskCreate(TaskMeasure, "Distance", 128, NULL, 1, NULL);
  vTaskStartScheduler();
}

void Taskdisplay(void * pvParameters) 
{
  int a;
  while(1) 
  {
  if ( xQueueReceive(queue_1, &a, portMAX_DELAY) == pdPASS) 
   {
  Serial.print("Distance: ");
  Serial.println(a);
  Serial.println(" cm");
  if(a < 20)
    {
          Serial.println("Dangerous!");
    
          digitalWrite(13, HIGH);
          digitalWrite(12, HIGH);
    }
    else 
    {
          Serial.println("Safe!");
         
          digitalWrite(13, LOW);
          digitalWrite(12, HIGH);
     }
     vTaskDelay( 500 / portTICK_PERIOD_MS );
   }
  }
}

void TaskMeasure(void * pvParameters) 
{
  long duration;  //biến đo thời gian
  int distance;   //biến đo khoảng cách
  while(1)     //tạo vòng lặp nếu true cho phát xung từ chân trig
  {
  digitalWrite(trigPin, LOW);  //tắt xung từ chân trig
  delayMicroseconds(2);        //xung có độ dài 5 microSeconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);   //Đo độ rộng xung HIGH ở chân echo (Hàm pulseIn() được dùng để đo độ rộng của xung)
  distance = (duration/2/29.412);      //Tính khoảng cách đến vật cản
  xQueueSend(queue_1, &distance, portMAX_DELAY);
  vTaskDelay( 1500 / portTICK_PERIOD_MS );
  }
}
