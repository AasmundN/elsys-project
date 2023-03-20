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
         <v-sheet
            class="pa-1 bg-grey-darken-4 rounded-lg matrixContainer"
            :style="{ maxWidth: 0.9 * width + 'px' }">
            <v-row no-gutters style="flex-wrap: nowrap" v-for="i in matrixSize.y" :key="i">
               <v-col
                  v-for="j in matrixSize.x"
                  :key="j"
                  class="ledContainer"
                  @mouseenter.prevent="
                     (event) => {
                        if (event.buttons) setLed(event)
                     }
                  "
                  @mousedown.prevent="setLed">
                  <div class="pa-2 ma-1 rounded-xl led" ref="ledElements"></div>
               </v-col>
            </v-row>
         </v-sheet>
      </v-row>
      <v-row no-gutters justify="center" class="mt-10">
         <v-col style="max-width: 400px" class="mx-6">
            <v-slider thumb-label v-model="speed" :min="0" :max="5" :step="1"></v-slider>
         </v-col>
      </v-row>
      <v-row no-gutters justify="center" class="my-6">
         <v-btn color="success" @click="submit" prepend-icon="mdi-cloud-upload">
            Send til hatt!
         </v-btn>
      </v-row>
   </v-container>
</template>

<script setup>
import { ref, computed } from "vue"
import { useDisplay } from "vuetify/lib/framework.mjs"

// matrix size
const matrixSize = { x: 19, y: 8 }
const enc = new TextEncoder()

const { width } = useDisplay()

const emit = defineEmits(["writeValue"])
const word = ref("")
const color = ref("")
const overlay = ref(false)
const ledElements = ref([])
const speed = ref(0)

let previousMatrix

const setLed = (event) => {
   let targetLedStyle = event.target.classList.contains("led")
      ? event.target.style
      : event.target.firstChild.style

   if (!targetLedStyle.backgroundColor) targetLedStyle.backgroundColor = `${color.value}`
   else targetLedStyle.backgroundColor = null
}

const submit = async () => {
   emit("writeValue", enc.encode(speed.value), "speed")

   const ledMatrix = ledElements.value.map((led) => led.style.backgroundColor)

   // if matrix is unchanged, do not update the matrix
   if (JSON.stringify(ledMatrix) === previousMatrix) return

   console.log("Start")

   await new Promise((resolve, reject) => {
      setTimeout(() => {
         resolve()
      }, 2000)
   })

   console.log("End")

   const byteStream = matrixToByteStream(ledMatrix)
   emit("writeValue", byteStream, "matrix")
   previousMatrix = JSON.stringify(ledMatrix)
}

const matrixToByteStream = (ledMatrix) => {
   let bytes = []
   let rgb

   ledMatrix.forEach((ledColor) => {
      if (ledColor === "") {
         for (let i = 0; i < 3; i++) bytes.push(0)
         return
      }

      rgb = ledColor.split("(")[1].split(")")[0].split(",")
      for (let i = 0; i < 3; i++) bytes.push(parseInt(rgb[i]))
   })

   return new Uint8Array(bytes).buffer
}

const clearMatrix = () => {
   ledElements.value.forEach((led) => (led.style.backgroundColor = null))
}
</script>

<style scoped>
.matrixContainer {
   overflow: scroll;
}

.matrixContainer::-webkit-scrollbar {
   display: none; /* for Chrome, Safari, and Opera */
}

.led {
   border-radius: 50%;
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
