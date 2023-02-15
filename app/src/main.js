import { createApp } from "vue"
import App from "./App.vue"
import router from "./router"
import store from "./store"

// Vuetify
import "vuetify/styles"
import { createVuetify } from "vuetify"
import * as components from "vuetify/components"
import * as directives from "vuetify/directives"

const vuetify = createVuetify({
   components,
   directives,
})

const app = createApp(App)

app.use(store)
app.use(router)
app.use(vuetify)

router.isReady().then(() => {
   app.mount("#app")
})
