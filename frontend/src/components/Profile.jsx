import { useState, useEffect } from 'react';
import { apiRequest } from '../api';

export default function Profile({ user, onUpdate }) {
  const [localUser, setLocalUser] = useState(() => {
    if (user && Object.keys(user).length > 0) return user;
    const saved = localStorage.getItem('user');
    try {
      return saved ? JSON.parse(saved) : null;
    } catch { return null; }
  });

  const [isEditing, setIsEditing] = useState(false);
  const [editForm, setEditForm] = useState({ username: '', email: '', age: '' });

  const activeUser = user || localUser;

  useEffect(() => {
    if (activeUser) {
      setEditForm({
        username: activeUser.username || activeUser.name || '',
        email: activeUser.email || '',
        age: activeUser.age || ''
      });
    }
  }, [user, localUser, isEditing]);

  const handleSave = async () => {
    try {
      const payload = {
        id: activeUser.id,
        username: editForm.username,
        email: editForm.email,
        age: editForm.age ? parseInt(editForm.age, 10) : null
      };

      const data = await apiRequest('PATCH', '/v1/users/patch', payload);
      
      const updatedUser = data.user || data;
      
      localStorage.setItem('user', JSON.stringify(updatedUser));
      onUpdate(updatedUser);
      setIsEditing(false);
      alert('Данные успешно обновлены!');
    } catch (err) {
      alert('Ошибка при сохранении: ' + err.message);
    }
  };

  if (!activeUser) {
    return <div style={{padding: '50px', color: 'black', textAlign: 'center'}}>Аккаунт не авторизован</div>;
  }

  const displayName = activeUser.username || activeUser.name || "Пользователь";
  const firstLetter = displayName[0]?.toUpperCase() || "U";

  return (
    <div style={profileCardStyle}>
      <div style={avatarStyle}>{firstLetter}</div>
      
      {!isEditing ? (
        <>
          <h2 style={{ marginBottom: '5px', color: '#2c3e50' }}>{displayName}</h2>
          <p style={{ color: '#3498db', fontWeight: 'bold', margin: '0 0 20px' }}>
            {(activeUser.role || 'user').toUpperCase()}
          </p>
          <div style={infoBoxStyle}>
            <div style={infoItemStyle}><strong>Email:</strong><span>{activeUser.email || '—'}</span></div>
            <div style={infoItemStyle}><strong>Возраст:</strong><span>{activeUser.age || '—'}</span></div>
            <div style={infoItemStyle}><strong>ID:</strong><span style={{fontSize: '10px'}}>{activeUser.id}</span></div>
          </div>
          <button style={editButtonStyle} onClick={() => setIsEditing(true)}>Настроить аккаунт</button>
        </>
      ) : (
        <div style={{ display: 'flex', flexDirection: 'column', gap: '15px' }}>
          <input style={inputStyle} value={editForm.username} onChange={e => setEditForm({...editForm, username: e.target.value})} placeholder="Имя" />
          <input style={inputStyle} value={editForm.email} onChange={e => setEditForm({...editForm, email: e.target.value})} placeholder="Email" />
          <input style={inputStyle} type="number" value={editForm.age} onChange={e => setEditForm({...editForm, age: e.target.value})} placeholder="Возраст" />
          <div style={{ display: 'flex', gap: '10px' }}>
            <button style={{ ...editButtonStyle, background: '#27ae60', flex: 1, marginTop: 0 }} onClick={handleSave}>Сохранить</button>
            <button style={{ ...editButtonStyle, background: '#95a5a6', flex: 1, marginTop: 0 }} onClick={() => setIsEditing(false)}>Отмена</button>
          </div>
        </div>
      )}
    </div>
  );
}

const profileCardStyle = { maxWidth: '450px', margin: '40px auto', padding: '30px', borderRadius: '16px', backgroundColor: '#fff', textAlign: 'center', boxShadow: '0 10px 25px rgba(0,0,0,0.1)' };
const avatarStyle = { width: '80px', height: '80px', backgroundColor: '#2c3e50', color: '#fff', borderRadius: '50%', display: 'flex', alignItems: 'center', justifyContent: 'center', fontSize: '30px', margin: '0 auto 20px' };
const infoBoxStyle = { textAlign: 'left', backgroundColor: '#f9f9f9', padding: '15px', borderRadius: '8px' };
const infoItemStyle = { display: 'flex', justifyContent: 'space-between', padding: '8px 0', borderBottom: '1px solid #eee' };
const editButtonStyle = { marginTop: '20px', padding: '12px', background: '#2c3e50', color: '#fff', border: 'none', borderRadius: '8px', cursor: 'pointer', width: '100%' };
const inputStyle = { padding: '10px', borderRadius: '6px', border: '1px solid #ddd' };