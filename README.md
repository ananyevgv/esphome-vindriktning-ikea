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

# ikea_external_components.yaml

```yaml
external_components:
  source: github://ananyevgv/esphome-opentherm-khenderick
  components: [opentherm]
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

