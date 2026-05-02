import { useState } from 'react';
import { apiRequest } from '../api';

export default function AddTractor({ onTractorAdded }) {
    const [form, setForm] = useState({
        id: '',
        lat: 55.75, 
        lon: 37.61,
        fuel_level: 100,
        status: 'active'
    });

    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const payload = {
                ...form,
                lat: parseFloat(form.lat),
                lon: parseFloat(form.lon),
                fuel_level: parseInt(form.fuel_level, 10)
            };

            await apiRequest('POST', '/v1/tractors/create', payload);
            
            setForm({ ...form, id: '' });
            onTractorAdded(); 
        } catch (err) {
            alert("Ошибка создания: " + err.message);
        }
    };

    return (
        <form onSubmit={handleSubmit} style={formStyle}>
            <input 
                placeholder="ID трактора (напр. tr_001)" 
                value={form.id}
                onChange={e => setForm({...form, id: e.target.value})}
                required 
            />
            <div style={{ display: 'flex', gap: '10px' }}>
                <input 
                    type="number" step="0.0001" placeholder="Широта (lat)" 
                    value={form.lat}
                    onChange={e => setForm({...form, lat: e.target.value})}
                    required 
                />
                <input 
                    type="number" step="0.0001" placeholder="Долгота (lon)" 
                    value={form.lon}
                    onChange={e => setForm({...form, lon: e.target.value})}
                    required 
                />
            </div>
            <input 
                type="number" placeholder="Уровень топлива (%)" 
                value={form.fuel_level}
                onChange={e => setForm({...form, fuel_level: e.target.value})}
                required 
            />
            <select value={form.status} onChange={e => setForm({...form, status: e.target.value})}>
                <option value="active">Активен</option>
                <option value="maintenance">ТО</option>
                <option value="broken">Сломан</option>
            </select>
            <button type="submit" style={{ backgroundColor: '#2c7a7b', color: 'white', border: 'none', padding: '10px' }}>
                Зарегистрировать технику
            </button>
        </form>
    );
}

const formStyle = { display: 'flex', flexDirection: 'column', gap: '10px', maxWidth: '400px' };