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
        const path = isLogin ? '/v1/users/register' : '/v1/users/register';
        
        const payload = { ...form };
        if (!isLogin) {
            payload.id = `user_${Math.floor(Math.random() * 1000)}`;
            if (payload.age) payload.age = parseInt(payload.age, 10);
            else delete payload.age;
        } else {
            delete payload.age;
            delete payload.role;
        }

        try {
            const data = await apiRequest('POST', path, payload);
            if (data.token) {
                localStorage.setItem('token', data.token);
                onLogin(data.token);
            }
        } catch (err) {
            alert('Ошибка: ' + err.message);
        }
    };

    return (
        <div style={authContainerStyle}>
            <h3>{isLogin ? 'Вход в систему' : 'Регистрация нового пользователя'}</h3>
            
            <form onSubmit={handleSubmit} style={formStyle}>
                <input 
                    placeholder="Имя пользователя" 
                    value={form.name}
                    onChange={e => setForm({...form, name: e.target.value})} 
                    required 
                />
                
                {!isLogin && (
                    <input 
                        type="email" 
                        placeholder="Email" 
                        onChange={e => setForm({...form, email: e.target.value})} 
                        required 
                    />
                )}
                
                <input 
                    type="password" 
                    placeholder="Пароль" 
                    onChange={e => setForm({...form, password: e.target.value})} 
                    required 
                />

                {!isLogin && (
                    <>
                        <input 
                            type="number" 
                            placeholder="Возраст (опционально)" 
                            onChange={e => setForm({...form, age: e.target.value})} 
                        />
                        <label>Ваша роль (по умолчанию зритель)</label>
                        <select value={form.role} onChange={e => setForm({...form, role: e.target.value})}>
                            <option value="viewer">Зритель</option>
                            <option value="mechanic">Механик</option>
                            <option value="admin">Администратор</option>
                        </select>
                    </>
                )}

                <button type="submit" style={buttonStyle}>
                    {isLogin ? 'Войти' : 'Создать аккаунт'}
                </button>
            </form>

            <p style={{ cursor: 'pointer', color: 'blue', marginTop: '15px' }} onClick={() => setIsLogin(!isLogin)}>
                {isLogin ? 'Нет аккаунта? Зарегистрируйтесь' : 'Уже есть аккаунт? Войдите'}
            </p>
        </div>
    );
}

const authContainerStyle = { padding: '20px', border: '1px solid #ccc', borderRadius: '8px', maxWidth: '350px', margin: 'auto' };
const formStyle = { display: 'flex', flexDirection: 'column', gap: '12px' };
const buttonStyle = { padding: '10px', background: '#007bff', color: 'white', border: 'none', borderRadius: '4px', cursor: 'pointer' };