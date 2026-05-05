export async function apiRequest(method, path, body = null) {
    const token = localStorage.getItem('token');
    const headers = { 'Content-Type': 'application/json' };
    if (token) headers['Authorization'] = `Bearer ${token}`;

    try {
        const response = await fetch(path, {
            method,
            headers,
            body: body ? JSON.stringify(body) : null
        });

        if (response.status === 401 || response.status === 403) {
            localStorage.clear(); 
            if (!window.location.pathname.includes('/auth')) {
                window.location.href = '/'; 
            }
            throw new Error("Доступ запрещен или сессия истекла");
        }

        if (!response.ok) {
            const errorData = await response.json().catch(() => ({}));
            throw new Error(errorData.message || `Ошибка сервера: ${response.status}`);
        }

        return response.status === 204 ? { success: true } : await response.json();

    } catch (err) {
        if (err.name === 'TypeError') {
            console.error("Критическая ошибка бэкенда (Socket hang up)");
        }
        throw err;
    }
}