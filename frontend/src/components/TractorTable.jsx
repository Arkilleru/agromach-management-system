export default function TractorTable({ tractors, isLoading, onDelete, isAdmin }) {
    return (
        <table style={{ width: '100%', borderCollapse: 'collapse', boxShadow: '0 2px 5px rgba(0,0,0,0.1)' }}>
            <thead>
                <tr style={{ backgroundColor: '#000000', textAlign: 'center', borderBottom: '2px solid #dee2e6' }}>
                    <th style={{ padding: '12px', width: '20%' }}>Название / ID</th>
                    <th style={{ padding: '12px', width: '40%' }}>Основная информация</th>
                    <th style={{ padding: '12px', width: '20%' }}>Статус</th>
                    <th style={{ padding: '12px', width: '20%' }}>Действия</th>
                </tr>
            </thead>
            <tbody>
                {tractors.length > 0 ? (
                    tractors.map((t) => (
                        <tr key={t.id} style={{ borderBottom: '1px solid #eee' }}>
                            <td style={{ padding: '12px' }}>
                                <div style={{ fontWeight: 'bold' }}>{t.name}</div>
                                <div style={{ fontSize: '0.8em', color: '#888', marginTop: '2px' }}><code>{t.id}</code></div>
                            </td>
                            
                            <td style={{ padding: '12px' }}>
                                <div style={{ fontSize: '0.9em', lineHeight: '1.4' }}>
                                    <div><strong>Координаты:</strong> {t.lat.toFixed(4)}, {t.lon.toFixed(4)}</div>
                                    <div><strong>Топливо:</strong> {t.fuel_level}%</div>
                                </div>
                            </td>
                            
                            <td style={{ padding: '12px' }}>
                                <span style={{ 
                                    padding: '4px 8px', 
                                    borderRadius: '4px', 
                                    fontSize: '0.85em',
                                    fontWeight: 'bold',
                                    backgroundColor: t.status === 'active' ? '#e6fffa' : '#fff5f5',
                                    color: t.status === 'active' ? '#3498db': '#c53030',
                                    textTransform: 'uppercase'
                                }}>
                                    {t.status}
                                </span>
                            </td>
                            
                            <td style={{ padding: '12px' }}>
                                {isAdmin && (
                                    <button
                                        onClick={() => onDelete(t.id)}
                                        style={deleteButtonStyle}
                                        onMouseOver={(e) => e.target.style.backgroundColor = '#c53030'}
                                        onMouseOut={(e) => e.target.style.backgroundColor = '#e53e3e'}
                                    >
                                        Удалить
                                    </button>
                                )}
                            </td>
                        </tr>
                    ))
                ) : (
                    <tr>
                        <td colSpan="4" style={{ textAlign: 'center', padding: '30px', color: '#888' }}>
                            {isLoading ? 'Загрузка данных...' : 'Список техники пуст'}
                        </td>
                    </tr>
                )}
            </tbody>
        </table>
    );
}

const deleteButtonStyle = { color: 'white', backgroundColor: '#e53e3e', border: 'none', padding: '6px 12px', borderRadius: '4px',cursor: 'pointer', transition: '0.2s'};