import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import './index.css';
import { invoke } from '@tauri-apps/api/tauri';

document.addEventListener('DOMContentLoaded', () => {
    invoke('close_splashscreen');
});

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
    <React.StrictMode>
        <App />
    </React.StrictMode>,
);
