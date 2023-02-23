<template>
   <v-container fluid class="h-100 pa-0">
      <v-row no-gutters justify="center" class="pa-8">
         <p class="text-center">Du er koblet til hatten!</p>
      </v-row>
      <v-row no-gutters justify="center">
         <v-responsive max-width="344" class="pa-2">
            <v-form validate-on="submit" @submit.prevent="submit">
               <v-text-field v-model="word" variant="outlined" label="Ord til hatt"></v-text-field>
               <v-btn type="submit" block class="mt-2" color="success">Send til hatt</v-btn>
            </v-form>
         </v-responsive>
      </v-row>
   </v-container>
</template>

<script setup>
import { ref } from "vue"

const emit = defineEmits(["writeValue"])

const word = ref("")

const submit = () => {
   emit("writeValue", word.value)
   word.value = ""
}
</script>
