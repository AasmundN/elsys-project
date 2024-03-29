<template>
   <v-app h-screen>
      <v-expansion-panels>
         <v-sheet
            width="100vw"
            height="36px"
            style="box-sizing: content-box"
            class="pa-2 d-flex justify-space-between align-center">
            <v-chip>Partyhatt</v-chip>
            <v-btn v-if="status" @click="disconnectDevice" color="red" height="100%"
               >Koble fra hatt</v-btn
            >
         </v-sheet>
      </v-expansion-panels>

      <v-main>
         <v-fade-transition leave-absolute>
            <Connect v-if="!status" :connecting="connecting" @connect="connectDevice" />
            <Mode v-else :mode="mode" @set-mode="setMode" />
         </v-fade-transition>

         <v-fade-transition leave-absolute>
            <template v-if="status">
               <keep-alive>
                  <component
                     :is="component"
                     @writeValue="
                        (value, characteristic) => writeCharacteristic(value, characteristic)
                     " />
               </keep-alive>
            </template>
         </v-fade-transition>
      </v-main>

      <v-snackbar v-model="snackbar" :timeout="4000">
         {{ message }}

         <template v-slot:actions>
            <v-btn color="blue" variant="text" @click="snackbar = false"> Lukk </v-btn>
         </template>
      </v-snackbar>
   </v-app>
</template>

<script setup>
import { ref, computed } from "vue"
import { useTheme } from "vuetify"

// components
import Connect from "./components/Connect.vue"
import Mode from "./components/Mode.vue"
import Matrix from "./components/Matrix.vue"
import Motion from "./components/Motion.vue"
import Sound from "./components/Sound.vue"

const serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

const characteristicUuids = {
   mode: "beb5483e-36e1-4688-b7f5-ea07361b26a8",
   fill: "e963c47e-c96a-4aee-8859-922adb4ac93a",
   matrix: "9f1b5ff8-b8ee-4e6c-b0be-668d85113b13",
   speed: "89177ae3-2410-4eeb-b484-75861c2e108a",
}

const enc = new TextEncoder()

const message = ref("")
const snackbar = ref(false)
const status = ref(false)
const connecting = ref(false)
const themeMode = ref(false)
// modes: sound motion matrix
const mode = ref("matrix")
const theme = useTheme()

let device, server, service
let characteristics = {
   mode: null,
   fill: null,
   matrix: null,
   speed: null,
}

const component = computed(() => {
   switch (mode.value) {
      case "matrix":
         return Matrix

      case "motion":
         return Motion

      case "sound":
         return Sound
   }
})

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
      // get characteristics
      for (const characteristic in characteristics) {
         characteristics[characteristic] = await service.getCharacteristic(
            characteristicUuids[characteristic]
         )
      }

      status.value = true
      connecting.value = false

      setMode(mode.value)
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
      fill: null,
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
      await characteristics[characteristic].writeValueWithResponse(value)
   } catch (error) {
      console.error(error)
      message.value = "Noe gikk galt, prøv igjen!"
      snackbar.value = true
   }
}
</script>
