#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t queue_1;

#define trigPin 3    // Trig 3
#define echoPin 4    // Echo 4
void setup() 
{
  Serial.begin(9600);
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (13, OUTPUT);
  
   queue_1 = xQueueCreate(5, sizeof(int));
  if (queue_1 == NULL)
  {
  Serial.println("Queue can not be created");
  }
  xTaskCreate(Taskdisplay, "Display_task", 128, NULL, 2, NULL);
  xTaskCreate(Taskdo, "Distance", 128, NULL, 1, NULL);
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
  if(a < 20)
    {
    Serial.println("Nguy Hiem");
    
    digitalWrite(13, HIGH);
    }
    else 
    {
          Serial.println("An toan");
         
          digitalWrite(13, LOW);
     }
     vTaskDelay( 500 / portTICK_PERIOD_MS );
   }
  }
}

void Taskdo(void * pvParameters) 
{
  long duration;
  int distance;
  while(1) 
  {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
   xQueueSend(queue_1, &distance, portMAX_DELAY);
        vTaskDelay( 1500 / portTICK_PERIOD_MS );
  }
}
