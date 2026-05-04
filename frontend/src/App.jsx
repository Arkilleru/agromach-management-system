import { useState, useEffect } from 'react';
import Auth from './components/Auth';
import Profile from './components/Profile';
import AddTractor from './components/AddTractor';
import TractorTable from './components/TractorTable';
import { apiRequest } from './api';

function App() {
    const [token, setToken] = useState(localStorage.getItem('token'));
    const [user, setUser] = useState(() => {
        const saved = localStorage.getItem('user');
        if (!saved || saved === "undefined") return null;
        try {
            return JSON.parse(saved);
        } catch (e) {
            return null;
        }
    });
    const [view, setView] = useState('tractors'); 
    const [tractors, setTractors] = useState([]);

    const handleLogin = (newToken, responseData) => { 
        if (!responseData || responseData.error) {
            console.error("Ошибка входа:", responseData?.error);
            return;
        }

        const userData = responseData.user || responseData; 
        
        setToken(newToken); 
        setUser(userData);
        
        localStorage.setItem('token', newToken);
        localStorage.setItem('user', JSON.stringify(userData));
    };

    const logout = () => { 
        localStorage.clear(); 
        setToken(null); 
        setUser(null); 
        setView('tractors'); 
    };

    const fetchTractors = async () => {
        try {
            const data = await apiRequest('GET', '/v1/tractors/lists');
            setTractors(Array.isArray(data) ? data : (data.tractors || []));
        } catch (err) { console.error(err); }
    };

    useEffect(() => { if (token) fetchTractors(); }, [token]);

    if (!token) return <Auth onLogin={handleLogin} />;

    return (
        <div style={appContainerStyle}>
            <header style={headerStyle}>
                <h2 style={{ margin: 0, cursor: 'pointer' }} onClick={() => setView('tractors')}>Agromach CMS</h2>
                <nav style={{ display: 'flex', gap: '25px', alignItems: 'center' }}>
                    <span style={view === 'tractors' ? activeTabStyle : navItemStyle} onClick={() => setView('tractors')}>Парк техники</span>
                    <span style={view === 'profile' ? activeTabStyle : navItemStyle} onClick={() => setView('profile')}>
                        Профиль: {user?.username || user?.name || user?.email || 'Загрузка...'}
                    </span>
                    <button onClick={logout} style={logoutBtnStyle}>Выйти</button>
                </nav>
            </header>

            <main style={mainStyle}>
                {view === 'tractors' ? (
                    <>
                        <section style={sectionStyle}>
                            <h3 style={{ marginTop: 0, color: '#fff' }}>Регистрация</h3>
                            <AddTractor onTractorAdded={fetchTractors} />
                        </section>
                        <TractorTable tractors={tractors} onDelete={fetchTractors} />
                    </>
                ) : (
                    <Profile user={user} onUpdate={(updated) => setUser(updated)} />
                )}
            </main>
        </div>
    );
}

const appContainerStyle = { fontFamily: 'system-ui, sans-serif', backgroundColor: '#f4f7f6', minHeight: '100vh' };
const headerStyle = { display: 'flex', justifyContent: 'space-between', alignItems: 'center', padding: '15px 40px', backgroundColor: '#2c3e50', color: 'white' };
const navItemStyle = { cursor: 'pointer' };
const activeTabStyle = { ...navItemStyle, borderBottom: '2px solid #3498db', color: '#3498db' };
const logoutBtnStyle = { backgroundColor: '#e74c3c', color: 'white', border: 'none', padding: '8px 16px', borderRadius: '5px', cursor: 'pointer' };
const mainStyle = { maxWidth: '1100px', margin: '40px auto', padding: '0 20px' };
const sectionStyle = { backgroundColor: '#000000', padding: '25px', borderRadius: '12px', marginBottom: '30px' };

export default App;