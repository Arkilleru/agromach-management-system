import { useState, useEffect } from 'react';
import { apiRequest } from '../api';

export default function UserManagement() {
    const [users, setUsers] = useState([]);

    const fetchUsers = async () => {
        try {
            const data = await apiRequest('GET', '/v1/users/list');
            setUsers(Array.isArray(data) ? data : []);
        } catch (err) {
            console.error("Failed to fetch users", err);
        }
    };

    useEffect(() => { fetchUsers(); }, []);

    const handleDelete = async (id) => {
        if (!window.confirm("Вы уверены, что хотите удалить этого пользователя?")) return;
        try {
            await apiRequest('DELETE', `/v1/users/delete?id=${id}`);
            fetchUsers();
        } catch (err) {
            alert("Ошибка при удалении: " + err.message);
        }
    };

    return (
        <div style={containerStyle}>
            <h3 style={{color: '#fff'}}>Управление пользователями</h3>
            <table style={tableStyle}>
                <thead>
                    <tr>
                        <th>Имя</th>
                        <th>Email</th>
                        <th>Роль</th>
                        <th>Действие</th>
                    </tr>
                </thead>
                <tbody>
                    {users.map(u => (
                        <tr key={u.id}>
                            <td>{u.username}</td>
                            <td>{u.email}</td>
                            <td><span style={roleBadge(u.role)}>{u.role}</span></td>
                            <td>
                                <button 
                                    onClick={() => handleDelete(u.id)}
                                    style={deleteBtnStyle}
                                >Удалить</button>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
}

const containerStyle = { backgroundColor: '#1a1a1a', padding: '20px', borderRadius: '12px' };
const tableStyle = { width: '100%', borderCollapse: 'collapse', marginTop: '20px', color: '#fff' };
const roleBadge = (role) => ({padding: '4px 8px', borderRadius: '4px', fontSize: '12px', backgroundColor: role === 'admin' ? '#e74c3c' : '#3498db'});
const deleteBtnStyle = { backgroundColor: '#e74c3c', color: '#fff', border: 'none', padding: '5px 10px', borderRadius: '4px', cursor: 'pointer' };