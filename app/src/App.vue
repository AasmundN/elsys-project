<template>
   <v-app h-screen>
      <v-expansion-panels>
         <v-expansion-panel
            title="MGP-hatt"
            text="Lorem ipsum dolor sit amet consectetur adipisicing elit. Commodi, ratione debitis quis est labore voluptatibus! Eaque cupiditate minima">
         </v-expansion-panel>
      </v-expansion-panels>

      <v-main>
         <v-switch
            v-model="themeMode"
            @update:model-value="toggleTheme"
            hide-details
            inset></v-switch>
      </v-main>
   </v-app>
</template>

<script setup>
import { ref, computed } from "vue"
import { useTheme } from "vuetify"

const themeMode = ref(false)
const theme = useTheme()

const toggleTheme = () => {
   theme.global.name.value = themeMode.value ? "light" : "dark"
}
</script>

<style scoped>
.switchWrapper {
   position: relative;
   left: 50px;
   bottom: 30px;
}
</style>
