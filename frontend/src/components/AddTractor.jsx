import { useState } from 'react';
import { apiRequest } from '../api';

export default function AddTractor({ onTractorAdded }) {
    const [form, setForm] = useState({
        name: '',
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
            
            setForm({
                name: '',
                lat: 55.75,
                lon: 37.61,
                fuel_level: 100,
                status: 'active'
            });
            onTractorAdded(); 
        } catch (err) {
            alert("Ошибка создания: " + err.message);
        }
    };

    return (
        <form onSubmit={handleSubmit} style={formStyle}>
            <input
                style={inputStyle}
                placeholder="Название трактора"
                value={form.name}
                onChange={e => setForm({...form, name: e.target.value})}
                required
            />
            <div style={{ display: 'flex', gap: '10px', width: '100%' }}>
                <input 
                    style={{...inputStyle, flex: 1}}
                    type="number" step="0.0001" placeholder="Широта (lat)" 
                    value={form.lat}
                    onChange={e => setForm({...form, lat: e.target.value})}
                    required 
                />
                <input 
                    style={{...inputStyle, flex: 1}}
                    type="number" step="0.0001" placeholder="Долгота (lon)" 
                    value={form.lon}
                    onChange={e => setForm({...form, lon: e.target.value})}
                    required 
                />
            </div>
            <input 
                style={inputStyle}
                type="number" placeholder="Уровень топлива (%)" 
                value={form.fuel_level}
                onChange={e => setForm({...form, fuel_level: e.target.value})}
                required 
            />
            <select style={inputStyle} value={form.status} onChange={e => setForm({...form, status: e.target.value})}>
                <option value="active">Активен</option>
                <option value="maintenance">ТО</option>
                <option value="broken">Сломан</option>
            </select>
            <button type="submit" style={buttonStyle}>
                Зарегистрировать технику
            </button>
        </form>
    );
}

const formStyle = { display: 'flex', flexDirection: 'column', gap: '12px', maxWidth: '450px', margin: '0 auto', alignItems: 'center', width: '100%'};
const inputStyle = { padding: '10px', borderRadius: '6px', border: '1px solid #444', backgroundColor: '#222', color: 'white', width: '100%', boxSizing: 'border-box', textAlign: 'center' };
const buttonStyle = { backgroundColor: '#3498db', color: 'white', border: 'none', padding: '12px 24px', borderRadius: '6px',cursor: 'pointer', fontWeight: 'bold', width: '100%', marginTop: '5px'};