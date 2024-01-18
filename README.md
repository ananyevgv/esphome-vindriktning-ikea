# esphome-vindriktning-ikea
| 1                                                    | 2                                                   | 
|------------------------------------------------------|-----------------------------------------------------|
| ![1](https://github.com/ananyevgv/esphome-vindriktning-ikea/blob/main/1704567361805.jpg) | ![2](https://github.com/ananyevgv/esphome-vindriktning-ikea/blob/main/1704567697413.jpg) |

Мой вариант vindriktning-ikea основан на https://modkam.ru/2023/10/30/ikea-vindriktning/
с небольшими  доработками, убран родной контроллер, добавлен датчик bme680, добавлено управление вентилятором и яркостью дисплея, измение цвета показаний в зависимости от значений, индикация солнца над горизонтом, индикация ожидаемой погоды в зависимости от давления.
Все размещено на одной странице дисплея.

# display_v2.yaml
<img src="https://github.com/ananyevgv/esphome-vindriktning-ikea/blob/main/v2.jpg" height="300" alt="v2">
<img src="https://github.com/ananyevgv/esphome-vindriktning-ikea/blob/main/ikea.jpg" height="300" alt="board">

# с родным контроллером
```yaml

uart:
  rx_pin: 1 #8 ножка контроллера
  baud_rate: 9600

external_components:
  source: github://ananyevgv/esphome-vindriktning-ikea
  components: [pm1006k]
  refresh: 0s

  sensor:
    - platform: pm1006k
      pm_1_0:   
        name: "PM 1.0"
        id: "pm1"
      pm_2_5:
        name: "PM 2.5"
        id: "pm2"
      pm_10_0:
        name: "PM 10"
        id: "pm10"

```
# External components PM1006k без родного контроллера


Если вы запускаете PM1006 (2,5 мкм) или PM1006K (1, 2,5, 10 мкм) без родного микроконтроллера, вам необходимо отправить последовательную команду в течение первых 5 секунд после подачи питания на датчик. В противном случае датчик переключится в режим PWM и не будет отвечать на запросы UART. 

```yaml
esphome:
  name: "${name}"
  on_boot:
    priority: 240
    then:
      - uart.write:
          id: PM1006k
          data: [0x11, 0x02, 0x0B, 0x01, 0xE1]
uart:
  tx_pin: 3 #1 ножка контроллера
  rx_pin: 1 #8 ножка контроллера
  baud_rate: 9600
  id: PM1006k

external_components:
  source: github://ananyevgv/esphome-vindriktning-ikea
  components: [pm1006k]
  refresh: 0s

  sensor:
    - platform: pm1006k
      pm_1_0:   
        name: "PM 1.0"
        id: "pm1"
      pm_2_5:
        name: "PM 2.5"
        id: "pm2"
      pm_10_0:
        name: "PM 10"
        id: "pm10"
      update_interval: 30s
```

