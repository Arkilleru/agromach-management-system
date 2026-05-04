import { useState } from 'react';
import { apiRequest } from '../api';

export default function Auth({ onLogin }) {
    const [isLogin, setIsLogin] = useState(true);
    const [form, setForm] = useState({ 
        name: '', 
        email: '', 
        password: '', 
        age: '', 
        role: 'viewer' 
    });

    const handleSubmit = async (e) => {
        e.preventDefault();
        
        const path = isLogin ? '/v1/login' : '/v1/users/register';
        let payload = {};
        
        if (isLogin) {
            payload = {
                email: form.email,
                password: form.password
            };
        } else {
            payload = { ...form };
            payload.id = `user_${Math.floor(Math.random() * 10000)}`;
            
            if (payload.age) {
                payload.age = parseInt(payload.age, 10);
            } else {
                delete payload.age;
            }
        }

        try {
            const data = await apiRequest('POST', path, payload);
            
            if (data && data.token) {
                onLogin(data.token, data);
            } else {
                alert('Ошибка: Сервер не вернул токен авторизации');
            }
        } catch (err) {
            console.error('Auth error:', err);
            alert('Ошибка: ' + (err.message || 'Неверные данные'));
        }
    };

    const handleChange = (field, value) => {
        setForm(prev => ({ ...prev, [field]: value }));
    };

    return (
        <div style={authContainerStyle}>
            <h2 style={{ textAlign: 'center', marginBottom: '20px', color: 'white' }}>
                {isLogin ? 'Вход в систему' : 'Регистрация'}
            </h2>
            
            <form onSubmit={handleSubmit} style={formStyle}>
                {!isLogin && (
                    <input 
                        style={inputStyle}
                        placeholder="Ваше имя" 
                        value={form.name}
                        onChange={e => handleChange('name', e.target.value)} 
                        required 
                    />
                )}
                
                <input 
                    style={inputStyle}
                    type="email" 
                    placeholder="Email" 
                    value={form.email}
                    onChange={e => handleChange('email', e.target.value)} 
                    required 
                />
                
                <input 
                    style={inputStyle}
                    type="password" 
                    placeholder="Пароль" 
                    value={form.password}
                    onChange={e => handleChange('password', e.target.value)} 
                    required 
                />

                {!isLogin && (
                    <>
                        <input 
                            style={inputStyle}
                            type="number" 
                            placeholder="Возраст (опционально)" 
                            value={form.age}
                            onChange={e => handleChange('age', e.target.value)} 
                        />
                        <div style={{ display: 'flex', flexDirection: 'column', gap: '5px' }}>
                            <label style={{ fontSize: '12px', color: '#bbb' }}>Выберите роль:</label>
                            <select 
                                style={inputStyle} 
                                value={form.role} 
                                onChange={e => handleChange('role', e.target.value)}
                            >
                                <option value="viewer">Зритель (Viewer)</option>
                                <option value="mechanic">Механик (Operator)</option>
                                <option value="admin">Администратор (Admin)</option>
                            </select>
                        </div>
                    </>
                )}

                <button type="submit" style={buttonStyle}>
                    {isLogin ? 'Войти' : 'Зарегистрироваться'}
                </button>
            </form>

            <p 
                style={{ cursor: 'pointer', color: '#3498db', marginTop: '20px', textAlign: 'center', fontSize: '14px' }} 
                onClick={() => setIsLogin(!isLogin)}
            >
                {isLogin ? 'Нет аккаунта? Зарегистрироваться' : 'Уже есть аккаунт? Войти'}
            </p>
        </div>
    );
}

const authContainerStyle = { padding: '30px', border: '1px solid #333', borderRadius: '12px', maxWidth: '400px', margin: '100px auto', boxShadow: '0 10px 30px rgba(0,0,0,0.5)', backgroundColor: '#1a1a1a' };
const formStyle = { display: 'flex', flexDirection: 'column', gap: '15px' };
const inputStyle = { padding: '12px', borderRadius: '6px', border: '1px solid #444', fontSize: '16px', backgroundColor: '#2b2b2b', color: 'white'};
const buttonStyle = { padding: '12px', background: '#3498db', color: 'white', border: 'none', borderRadius: '6px', cursor: 'pointer', fontSize: '16px', fontWeight: 'bold', marginTop: '10px'};