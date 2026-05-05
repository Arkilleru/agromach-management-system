import { useState, useEffect, useCallback } from 'react';
import Auth from './components/Auth';
import Profile from './components/Profile';
import AddTractor from './components/AddTractor';
import TractorTable from './components/TractorTable';
import UserManagement from './components/UserManagement';
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
    const [isChecking, setIsChecking] = useState(false);

    const fetchTractors = useCallback(async () => {
        if (!token) return;
        try {
            const data = await apiRequest('GET', '/v1/tractors/lists');
            setTractors(Array.isArray(data) ? data : (data.tractors || []));
        } catch (err) { 
            console.error("Ошибка загрузки техники:", err.message);
        }
    }, [token]);

    const logout = () => { 
        localStorage.clear(); 
        setToken(null); 
        setUser(null); 
        setView('tractors'); 
    };

    const handleLogin = (newToken, responseData) => { 
        const actualToken = newToken || responseData.token;
        const userData = responseData.user || responseData; 

        if (!actualToken) {
            console.error("Токен не получен");
            return;
        }

        localStorage.setItem('token', actualToken);
        localStorage.setItem('user', JSON.stringify(userData));
        
        setToken(actualToken); 
        setUser(userData);
    };

    useEffect(() => {
        const validateUser = async () => {
            if (!token) return;

            try {
                setIsChecking(true);
                const currentUser = await apiRequest('GET', '/v1/users/me');
                const userData = currentUser.user || currentUser;
                setUser(userData);
                localStorage.setItem('user', JSON.stringify(userData));
            } catch (err) {
                console.error("Проверка сессии провалена:", err.message);
                if (err.message.includes('401') || err.message.includes('Сессия')) {
                    logout();
                }
            } finally {
                setIsChecking(false);
            }
        };

        validateUser();
    }, [token]);

    useEffect(() => { 
        if (token && !isChecking) {
            fetchTractors(); 
        }
    }, [token, isChecking, fetchTractors]);
-

    const isAdmin = user?.role === 'admin';

    if (!token) {
        return <Auth onLogin={handleLogin} />;
    }

    if (isChecking && !user) {
        return (
            <div style={{ padding: '50px', textAlign: 'center', color: '#2c3e50' }}>
                <h2>Связь с сервером...</h2>
            </div>
        );
    }

    return (
        <div style={appContainerStyle}>
            <header style={headerStyle}>
                <h2 style={{ margin: 0, cursor: 'pointer' }} onClick={() => setView('tractors')}>
                    Agromach CMS
                </h2>
                <nav style={{ display: 'flex', gap: '25px', alignItems: 'center' }}>
                    <span 
                        style={view === 'tractors' ? activeTabStyle : navItemStyle} 
                        onClick={() => setView('tractors')}
                    >
                        Парк техники
                    </span>

                    {isAdmin && (
                        <span 
                            style={view === 'users' ? activeTabStyle : navItemStyle} 
                            onClick={() => setView('users')}
                        >
                            Пользователи
                        </span>
                    )}

                    <span 
                        style={view === 'profile' ? activeTabStyle : navItemStyle} 
                        onClick={() => setView('profile')}
                    >
                        Профиль: {user?.username || user?.name || 'Загрузка...'}
                    </span>
                    <button onClick={logout} style={logoutBtnStyle}>Выйти</button>
                </nav>
            </header>

            <main style={mainStyle}>
                {view === 'tractors' && (
                    <>
                        <section style={sectionStyle}>
                            <h3 style={{ marginTop: 0, color: '#fff' }}>Регистрация техники</h3>
                            <AddTractor onTractorAdded={fetchTractors} />
                        </section>
                        <TractorTable tractors={tractors} onDelete={fetchTractors} />
                    </>
                )}

                {view === 'users' && isAdmin && (
                    <UserManagement />
                )}

                {view === 'profile' && (
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