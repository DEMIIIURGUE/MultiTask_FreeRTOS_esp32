#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Object
int lcd_count;
#define ledEsp 2 // Define led pin 2 esp32.
int state_ledEsp = 0;

TaskHandle_t Tarea0;
TaskHandle_t Tarea1;

void loop0_Tarea0(void *parameter)
{
  const TickType_t xDelay1 = 4000 / portTICK_PERIOD_MS; // delay en ms.
  while (1)
  {
    lcd_count = lcd_count + 1;
    lcd.print(lcd_count);
    vTaskDelay(xDelay1);
    lcd.clear();
  }
}
void loop1_Tarea1(void *parameter)
{
  const TickType_t xDelay1 = 200 / portTICK_PERIOD_MS; // delay en ms.
  while (1)
  {
    state_ledEsp = !state_ledEsp;
    digitalWrite(ledEsp, state_ledEsp);
    vTaskDelay(xDelay1);
  }
}

void setup()
{
  xTaskCreatePinnedToCore(loop0_Tarea0, "Tarea_0", 4096, NULL, 1, &Tarea0, 1); // Core 1
  xTaskCreatePinnedToCore(loop1_Tarea1, "Tarea_1", 4096, NULL, 1, &Tarea1, 1); // Core 1
  pinMode(2, OUTPUT);
  // Inicializa función LCD:  --------------------------------------------
  lcd.init();
  lcd.backlight();
}
void loop()
{
  if (lcd_count % 2 == 1) // Si es PAR suspende la tarea.
  {
    vTaskSuspend(Tarea1); // Suspende tarea.
  }
  else
  {
    vTaskResume(Tarea1); // Inicia tarea suspendida.
  }
}
