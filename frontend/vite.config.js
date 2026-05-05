import { defineConfig } from 'vite';

export default defineConfig({
  server: {
    host: true, 
    port: 5173,
    proxy: {
      '/v1': {
        target: 'http://agromach-server:8080', 
        changeOrigin: true,
      },
    },
  },
});