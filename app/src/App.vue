<template>
   <div>
      <h1>Elsys bois</h1>
   </div>
   <button @click="connect">Connect</button>

   <p v-if="connectedDevice">Connected to {{ connectedDevice }}</p>
</template>

<script setup>
import { ref } from "vue"

const connectedDevice = ref("")
const serviceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
const characteristicUuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const connect = () => {
   navigator.bluetooth
      .requestDevice({
         filters: [{ services: [serviceUuid] }],
      })
      .then((device) => {
         console.log(device)
         connectedDevice.value = device.name
         return device.gatt.connect()
      })
      .then((server) => {
         console.log("Getting Service...")
         return server.getPrimaryService(serviceUuid)
      })
      .then((service) => {
         console.log("Getting Characteristic...")
         return service.getCharacteristic(characteristicUuid)
      })
      .then((characteristic) => {
         const myCharacteristic = characteristic
         return myCharacteristic.startNotifications().then(() => {
            console.log("> Notifications started")
            myCharacteristic.addEventListener("characteristicvaluechanged", handleNotifications)
         })
      })
      .catch((error) => {
         console.log("Argh! " + error)
      })
}

const handleNotifications = (event) => {
   let value = event.target.value
   let a = []
   // Convert raw data bytes to hex values just for the sake of showing something.
   // In the "real" world, you'd use data.getUint8, data.getUint16 or even
   // TextDecoder to process raw data bytes.
   for (let i = 0; i < value.byteLength; i++) {
      a.push("0x" + ("00" + value.getUint8(i).toString(16)).slice(-2))
   }
   console.log("> " + a.join(" "))
}
</script>

<style>
div {
   display: flex;
   flex-direction: column;
   align-items: center;
   justify-content: center;
   align-content: center;
}
button {
   background-color: white !important;

   padding: 5px 10px;
   cursor: pointer;
}
</style>
