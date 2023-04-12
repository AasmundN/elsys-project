<template>
   <v-container class="pa-0">
      <v-row no-gutters justify="center">
         <v-btn color="success" @click="submit(false)" prepend-icon="mdi-cloud-upload">
            Oppdater hatt
         </v-btn>
      </v-row>

      <v-row no-gutters justify="center" class="mt-5">
         <v-sheet
            class="pa-1 bg-grey-darken-4 rounded-lg matrixContainer"
            :style="{ maxWidth: 0.9 * width + 'px', overflow: mobile ? 'scroll' : 'hidden' }">
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

      <v-row v-if="mobile" no-gutters justify="center" class="mt-5">
         <v-chip class="ma-2" closable label> Dra til høyre for å se hele matrisen </v-chip>
      </v-row>

      <v-row no-gutters justify="center" class="mt-5">
         <v-btn :color="color" @click="overlay = !overlay" class="mx-2"> Endre farge </v-btn>
         <v-overlay v-model="overlay" class="d-flex justify-center align-center">
            <v-row no-gutters justify="center" align-content="center">
               <v-color-picker v-model="color" mode="rgb" :modes="['rgb']"></v-color-picker>
            </v-row>
         </v-overlay>
         <v-btn
            @click="
               () => {
                  selectSpeed = !selectSpeed
                  speed = 0
               }
            "
            :color="selectSpeed ? 'grey-lighten-2' : 'default'">
            Animer
         </v-btn>
         <v-btn class="mx-2" color="red" @click="clearMatrix" prepend-icon="mdi-close"> Tøm </v-btn>
      </v-row>
      <v-fade-transition>
         <v-row v-if="selectSpeed" no-gutters justify="center" class="mt-5 px-4">
            <v-chip> Hastighet </v-chip>
            <v-slider
               thumb-label
               v-model="speed"
               :min="-3"
               :max="3"
               :step="1"
               show-ticks="always"
               style="max-width: 400px" />
         </v-row>
      </v-fade-transition>
   </v-container>
</template>

<script setup>
import { ref, onUpdated, onMounted } from "vue"
import { useDisplay } from "vuetify/lib/framework.mjs"

// matrix size
const matrixSize = { x: 19, y: 8 }
const enc = new TextEncoder()

const { width, mobile } = useDisplay()

const emit = defineEmits(["writeValue"])
const color = ref("#0055DD")
const overlay = ref(false)
const ledElements = ref([])
const speed = ref(0)
const selectSpeed = ref(false)

let previousMatrix

const setLed = (event) => {
   let targetLedStyle = event.target.classList.contains("led")
      ? event.target.style
      : event.target.firstChild.style

   if (!targetLedStyle.backgroundColor) targetLedStyle.backgroundColor = `${color.value}`
   else targetLedStyle.backgroundColor = null
}

const submit = async (force) => {
   emit("writeValue", enc.encode(speed.value), "speed")

   const ledMatrix = ledElements.value.map((led) => led.style.backgroundColor)

   // if matrix is unchanged, do not update the matrix
   if (JSON.stringify(ledMatrix) === previousMatrix && !force) return

   // give the bluetooth stack time to get ready for new transmission
   await new Promise((resolve, reject) => {
      setTimeout(() => {
         resolve()
      }, 200)
   })

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

onMounted(() => submit(true))
onUpdated(() => submit(true))
</script>

<style scoped>
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
