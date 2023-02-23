<template>
   <v-app h-screen>
      <v-expansion-panels>
         <v-expansion-panel
            title="MGP-hatt"
            text="Lorem ipsum dolor sit amet consectetur adipisicing elit. Commodi, ratione debitis quis est labore voluptatibus! Eaque cupiditate minima">
         </v-expansion-panel>
      </v-expansion-panels>

      <v-main>
         <connect v-if="!status" :connecting="connecting" @connect="connectDevice" />
         <mode v-else @writeValue="writeCharacteristic" />
      </v-main>

      <div class="d-flex justify-center align-center px-5 py-2">
         <v-switch
            v-model="themeMode"
            @update:model-value="toggleTheme"
            hide-details
            inset></v-switch>
         <v-btn v-if="status" @click="disconnectDevice" color="red">Koble fra hatt</v-btn>
      </div>

      <v-snackbar v-model="snackbar" :timeout="2500">
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
import connect from "./views/Connect.vue"
import mode from "./views/Mode.vue"

const serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
const characteristicUuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const dec = new TextDecoder()
const enc = new TextEncoder()

const message = ref("")
const snackbar = ref(false)
const status = ref(false)
const connecting = ref(false)
const themeMode = ref(false)
const theme = useTheme()

let device, server, service, characteristic

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
      // get characteristic
      characteristic = await service.getCharacteristic(characteristicUuid)
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
   console.log("Device disconnected")
   status.value = false
   message.value = "Koblet fra hatt!"
   snackbar.value = true
   device = server = service = characteristic = null
}

const getCharacteristicValue = async () => {
   if (!status.value) return
   const DataView = await characteristic.readValue()
   return convertValue(DataView)
}

const writeCharacteristic = async (value) => {
   if (!status.value) return
   await characteristic.writeValueWithResponse(enc.encode(value))
   message.value = "Ord sendt til hatt!"
   snackbar.value = true
}

const convertValue = (DataView) => {
   return dec.decode(DataView.buffer)
}
</script>
