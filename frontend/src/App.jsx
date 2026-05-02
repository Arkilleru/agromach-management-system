import { useState, useEffect } from 'react';
import Auth from './components/Auth';
import AddTractor from './components/AddTractor';
import TractorTable from './components/TractorTable';
import { apiRequest } from './api';

function App() {
    const [token, setToken] = useState(localStorage.getItem('token'));
    const [tractors, setTractors] = useState([]);
    const [isLoading, setIsLoading] = useState(false);

    const fetchTractors = async () => {
        setIsLoading(true);
        try {
            const data = await apiRequest('GET', '/v1/tractors/lists');
            const list = Array.isArray(data) ? data : (data.tractors || []);
            setTractors(list);
        } catch (err) {
            console.error('Ошибка загрузки списка:', err);
        } finally {
            setIsLoading(false);
        }
    };

    const logout = () => {
        localStorage.removeItem('token');
        setToken(null);
    };

    const handleDelete = async (id) => {
        if (!window.confirm(`Удалить трактор ${id}?`)) return;
        try {
            await apiRequest('DELETE', `/v1/tractor/delete?id=${id}`); 
            fetchTractors(); 
        } catch (err) {
            alert("Ошибка: Действие разрешено только администратору!");
        }
    };

    useEffect(() => {
        if (token) fetchTractors();
    }, [token]);

    if (!token) return <div style={{ padding: '40px', textAlign: 'center' }}><Auth onLogin={setToken} /></div>;

    return (
        <div style={{ padding: '20px', fontFamily: 'system-ui, sans-serif', maxWidth: '1000px', margin: '0 auto' }}>
            <header style={headerStyle}>
                <h1 style={{ margin: 0 }}>Agromach CMS</h1>
                <div>
                    <span style={{ marginRight: '15px', color: '#2c7a7b', fontWeight: 'bold' }}>● Online</span>
                    <button onClick={logout} style={{ cursor: 'pointer' }}>Выйти</button>
                </div>
            </header>

            <section style={sectionStyle}>
                <h3 style={{ marginTop: 0 }}>Регистрация новой единицы</h3>
                <AddTractor onTractorAdded={fetchTractors} />
            </section>

            <TractorTable 
                tractors={tractors} 
                isLoading={isLoading} 
                onDelete={handleDelete} 
            />
        </div>
    );
}

const headerStyle = { display: 'flex', justifyContent: 'space-between', alignItems: 'center', borderBottom: '0px solid #eee', marginBottom: '0px', paddingBottom: '10px' };
const sectionStyle = { backgroundColor: '#000000', padding: '20px', border: '0px solid #eee', borderRadius: '0 0 8px 8px', marginBottom: '30px' };

export default App;