![image](https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D)
![image](https://img.shields.io/badge/Vuetify-1867C0?style=for-the-badge&logo=vuetify&logoColor=white)

### Elsys project <img style="position: relative; top: 18px;" width="50" height="50" src="https://scontent.xx.fbcdn.net/v/t1.15752-9/339065060_1997846673886286_913586771088909749_n.png?_nc_cat=109&ccb=1-7&_nc_sid=aee45a&_nc_ohc=N9sq403BY-YAX_KqNql&_nc_ad=z-m&_nc_cid=0&_nc_ht=scontent.xx&oh=03_AdQykzMZ4O2XJBAsQrDeDKPi3QRYSgRhn3hvxAC96559Dw&oe=645F3820">

---

Project repository for NTNU subject _TT4270 Elsys prosjekt_ spring 2023.

🔗 [Live web application ](https://partyhatt.site/)

### Frameworks

---

-  [Vue 3](https://vuejs.org/) - Progressive JavaScript Framework
-  [Vuetify 3](https://next.vuetifyjs.com/en/) - Material Design Framework
-  [PlatformIO](https://platformio.org/) - Embedded development in VSCode

### Web application setup

---

```sh
cd app
```

```sh
npm install
```

```sh
npm run serve
```

### Microcontroller setup

---

The microcontroller used in this project is an [ESP32](https://en.wikipedia.org/wiki/ESP32). To upload the project code to your microcontroller, first install [PlatformIO](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) in [VSCode](https://code.visualstudio.com/). Open the repository directory [main](main/). You can now upload the project.

### Sensor setup

---

The sensors used for the project are the [Max9814](https://www.fruugonorge.com/botao-max9814-electret-mic-microphone-amplifier-module-auto-gain-control/p-118346267-248653265?language=en&ac=ProductCasterAPI&asc=pmax&gclid=CjwKCAjw0N6hBhAUEiwAXab-TdKtCogrgaKGedufb5ztdlp024D4YnC2fW9KTFl-NPYy-jNSEylLVBoC07kQAvD_BwE) microphone and the [MMA8451](<https://no.rs-online.com/web/p/motion-sensor-ics/9054665?cm_mmc=NO-PLA-DS3A-_-google-_-PLA_NO_NO_Semiconductors_Whoop-_-(NO:Whoop!)+Motion+Sensor+ICs-_-9054665&matchtype=&pla-327734554961&gclid=CjwKCAjw0N6hBhAUEiwAXab-TRhGJnz1WGHHL0heU1fYiKufu3wxTizIHyHHvrfqkiPikoXhszkTYhoCV1wQAvD_BwE&gclsrc=aw.ds>) gyroscope, both from [Aadafruit](https://www.adafruit.com/category/521). The pin setup is specified in the corresponding program files.
