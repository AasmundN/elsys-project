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

const connect = () => {
   navigator.bluetooth
      .requestDevice({
         acceptAllDevices: true,
      })
      .then((device) => {
         console.log(device)
         connectedDevice.value = device.name
      })
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
   background-color: white;
   padding: 5px 10px;
   cursor: pointer;
}
</style>
