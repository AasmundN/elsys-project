<template>
   <v-app h-screen>
      <v-expansion-panels>
         <v-expansion-panel
            title="Swizz party hatt!"
            text="Dette er en kort beskrivelse av hva Hatten er, hvordan man bruker den, og hvorfor man bruker den. Vi håper at alt er gøy og trivelig og at ingenting slutter å virke.">
         </v-expansion-panel>
      </v-expansion-panels>

      {{ debug1 }} <br />
      {{ debug2 }} <br />
      {{ debug3 }} <br />

      <v-main>
         <v-fade-transition leave-absolute>
            <Connect v-if="!status" :connecting="connecting" @connect="connectDevice" />
            <Mode v-else :mode="mode" @set-mode="setMode" />
         </v-fade-transition>

         <v-fade-transition leave-absolute>
            <Matrix
               v-if="mode === 'matrix' && status"
               @writeValue="
                  (value, characteristic) => writeCharacteristic(value, characteristic)
               " />
            <ColorPicker
               v-if="mode != 'matrix' && status"
               @write-value="(newColor) => writeCharacteristic(enc.encode(newColor), 'color')" />
         </v-fade-transition>
      </v-main>

      <div class="d-flex justify-center align-center px-5 py-2">
         <v-switch
            v-model="themeMode"
            @update:model-value="toggleTheme"
            hide-details
            inset></v-switch>
         <v-btn v-if="status" @click="disconnectDevice" color="red">Koble fra hatt</v-btn>
      </div>

      <v-snackbar v-model="snackbar" :timeout="4000">
         {{ message }}

         <template v-slot:actions>
            <v-btn color="blue" variant="text" @click="snackbar = false"> Lukk </v-btn>
         </template>
      </v-snackbar>
   </v-app>
</template>

<script setup>
import { ref } from "vue"
import { useTheme } from "vuetify"

// components
import Connect from "./components/Connect.vue"
import Mode from "./components/Mode.vue"
import Matrix from "./components/Matrix.vue"
import ColorPicker from "./components/ColorPicker.vue"

const serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

const characteristicUuids = {
   mode: "beb5483e-36e1-4688-b7f5-ea07361b26a8",
   color: "e963c47e-c96a-4aee-8859-922adb4ac93a",
   matrix: "9f1b5ff8-b8ee-4e6c-b0be-668d85113b13",
   speed: "89177ae3-2410-4eeb-b484-75861c2e108a",
}

const dec = new TextDecoder()
const enc = new TextEncoder()

const message = ref("")
const snackbar = ref(false)
const status = ref(false)
const connecting = ref(false)
const themeMode = ref(false)
// modes: sound motion matrix
const mode = ref("sound")
const theme = useTheme()

const debug1 = ref()
const debug2 = ref()
const debug3 = ref()

let device, server, service
let characteristics = {
   mode: null,
   color: null,
   matrix: null,
   speed: null,
}

const toggleTheme = () => {
   theme.global.name.value = themeMode.value ? "light" : "dark"
}

const connectDevice = async () => {
   connecting.value = true
   try {
      // scan for device
      device = await navigator.bluetooth.requestDevice({
         filters: [{ services: [serviceUuid] }],
      })
      device.addEventListener("gattserverdisconnected", onDisconnect)
      // connect to BLE server
      server = await device.gatt.connect()
      // get service
      service = await server.getPrimaryService(serviceUuid)

      for (const characteristic in characteristics) {
         characteristics[characteristic] = await service.getCharacteristic(
            characteristicUuids[characteristic]
         )
      }

      status.value = true
      connecting.value = false
   } catch (error) {
      connecting.value = false
      message.value = "Noe gikk galt, prøv igjen!"
      snackbar.value = true
   }
}

const disconnectDevice = () => {
   if (!device?.gatt.connected) return
   device.gatt.disconnect()
}

const onDisconnect = () => {
   status.value = false
   message.value = "Koblet fra hatt!"
   snackbar.value = true
   device = server = service = null
   characteristics = {
      mode: null,
      color: null,
      matrix: null,
      speed: null,
   }
}

const setMode = (newMode) => {
   mode.value = newMode
   writeCharacteristic(enc.encode(newMode), "mode")
}

const writeCharacteristic = async (value, characteristic) => {
   if (!status.value) return
   try {
      debug1.value = characteristic
      debug3.value = characteristics[characteristic]
      console.log(value)
      await characteristics[characteristic].writeValueWithResponse(value)
      if (characteristic === "matrix") {
         debug1.value = "Matrix updated"
      }
   } catch (error) {
      console.error(error)
      debug1.value = "Something went wrong"
      debug2.value = error
      message.value = "Noe gikk galt, prøv igjen!"
      snackbar.value = true
   }
}
</script>
