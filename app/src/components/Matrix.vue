<template>
   <v-container class="pa-0">
      <v-row no-gutters justify="center">
         <v-btn :color="color" @click="overlay = !overlay" class="mx-2"> Velg farge </v-btn>
         <v-overlay v-model="overlay" class="d-flex justify-center align-center">
            <v-row no-gutters justify="center" align-content="center">
               <v-color-picker v-model="color"></v-color-picker>
            </v-row>
         </v-overlay>
         <v-btn class="mx-2" color="red" @click="clearMatrix" prepend-icon="mdi-close"> Tøm </v-btn>
      </v-row>
      <v-row no-gutters justify="center" class="mt-10">
         <v-sheet class="rounded-lg pa-1 bg-grey-darken-4">
            <v-row no-gutters v-for="i in matrixSize.y" :key="i">
               <v-col
                  v-for="j in matrixSize.x"
                  :key="j"
                  class="ledContainer"
                  @mouseenter="
                     (event) => {
                        if (event.buttons) setLed(event)
                     }
                  "
                  @mousedown="setLed">
                  <div class="pa-2 ma-1 rounded-xl led" ref="ledElements"></div>
               </v-col>
            </v-row>
         </v-sheet>
      </v-row>
      <v-row no-gutters justify="center" class="mt-10">
         <v-col cols="2">
            <v-slider thumb-label :min="0" :max="5" :step="1"></v-slider>
         </v-col>
      </v-row>
      <v-row no-gutters justify="center" class="mt-8">
         <v-btn color="success" @click="submit" prepend-icon="mdi-cloud-upload">
            Send til hatt!
         </v-btn>
      </v-row>
   </v-container>
</template>

<script setup>
import { ref, computed } from "vue"

// matrix size
const matrixSize = { x: 19, y: 8 }

const emit = defineEmits(["writeValue"])
const word = ref("")
const color = ref("")
const overlay = ref(false)
const ledElements = ref([])

const setLed = (event) => {
   let targetLedStyle = event.target.classList.contains("led")
      ? event.target.style
      : event.target.firstChild.style

   if (!targetLedStyle.backgroundColor) targetLedStyle.backgroundColor = `${color.value}`
   else targetLedStyle.backgroundColor = null
}

const submit = () => {
   let ledMatrix = ledElements.value.map((led) => rgbToHex(led.style.backgroundColor))
   let matrixHexString = matrixToHexString(ledMatrix)
   emit("writeValue", hexDecode(matrixHexString))
}

const clearMatrix = () => {
   ledElements.value.forEach((led) => (led.style.backgroundColor = null))
}

const matrixToHexString = (matrix) => {
   let result = ""
   matrix.forEach((ledColor) => {
      result += ledColor
   })
   return result
}

const hexDecode = (hexString) => {
   let bytes = []
   hexString.replace(/../g, (pair) => {
      bytes.push(parseInt(pair, 16))
   })
   return new Uint8Array(bytes).buffer
}

const rgbToHex = (rgb) => {
   if (!rgb) return "000000"
   let sep = rgb.indexOf(",") > -1 ? "," : " "
   rgb = rgb.substr(4).split(")")[0].split(sep)
   return (
      parseInt(rgb[0]).toString(16) + parseInt(rgb[1]).toString(16) + parseInt(rgb[2]).toString(16)
   )
}
</script>

<style scoped>
.led {
   box-sizing: border-box;
   border: 2px #757575 solid;
}

.ledContainer {
   cursor: pointer;
}

.ledContainer:hover .led {
   border: 2px white solid;
}
</style>
