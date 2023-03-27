<template>
   <v-container class="pa-0">
      <v-row no-gutters justify="center" class="px-3 mb-5">
         <p class="text-center">Under kan du velge fargen på lysene.</p>
      </v-row>
      <v-row no-gutters justify="center">
         <v-color-picker v-model="color" mode="rgb" :modes="['rgb']"></v-color-picker>
      </v-row>
      <v-row no-gutters justify="center" class="mt-10">
         <v-btn color="success" @click="submit" prepend-icon="mdi-cloud-upload">
            Oppdater farge!
         </v-btn>
      </v-row>
      <v-snackbar v-model="snackbar" :timeout="4000">
         Du må velge en farge

         <template v-slot:actions>
            <v-btn color="blue" variant="text" @click="snackbar = false"> Lukk </v-btn>
         </template>
      </v-snackbar>
   </v-container>
</template>

<script setup>
import { ref } from "vue"

const color = ref("#FF0000")
const snackbar = ref(false)

const emit = defineEmits(["writeValue"])

const submit = () => {
   if (!color.value) {
      snackbar.value = true
      return
   }

   let colorIntArr = color.value
      .substring(1, color.value.length)
      .match(/.{1,2}/g)
      .map((val) => parseInt(val, 16))

   emit("writeValue", colorIntArr.slice(0, 3).join(","))
}
</script>
