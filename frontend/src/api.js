export async function apiRequest(method, path, body = null) {
    const token = localStorage.getItem('token');
    const headers = {
        'Content-Type': 'application/json',
    };
    if (token) headers['Authorization'] = `Bearer ${token}`;

    const response = await fetch(path, {
        method,
        headers,
        body: body ? JSON.stringify(body) : null
    });

    if (!response.ok) {
        if (response.status === 403) {
            throw new Error("У вас недостаточно прав для этого действия (нужна роль operator или выше)");
        }
        if (response.status === 401) {
            throw new Error("Сессия истекла. Пожалуйста, войдите снова");
        }
        
        const errorData = await response.json().catch(() => ({}));
        throw new Error(errorData.message || `Ошибка сервера: ${response.status}`);
    }

    return response.json();
}