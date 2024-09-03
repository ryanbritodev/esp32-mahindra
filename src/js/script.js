// Wokwi Simulation: https://wokwi.com/projects/407869921950393345

const channelID = '2642712'; // ID do Canal ThingSpeak
const readAPIKey = 'QD97WQCRFVW3D4DZ'; // Read API Key do Canal

function getData() {
    document.getElementById('loading').style.display = 'block';
    fetch(`https://api.thingspeak.com/channels/${channelID}/feeds.json?api_key=${readAPIKey}&results=1`)
    .then(response => response.json())
    .then(data => {
        const feed = data.feeds[0];
        const temp = parseFloat(feed.field1).toFixed(0); // Temperatura
        const humidity = parseFloat(feed.field2).toFixed(0); // Umidade
        const speed = parseFloat(feed.field3).toFixed(0); // Velocidade

        document.getElementById('temp').textContent = `${temp}°C`;
        document.getElementById('humidity').textContent = `${humidity}%`;
        document.getElementById('speed').textContent = `${speed}km/h`;

        document.getElementById('loading').style.display = 'none';
    })
    .catch(error => {
        console.error('Erro ao obter dados do ThingSpeak:', error);
        document.getElementById('data').innerHTML = '<p class="error">Erro ao carregar dados.</p>';
        document.getElementById('loading').style.display = 'none';
    });
}

getData();
setInterval(getData, 15000); // Atualiza os dados a cada 15 segundos

function updateTime() {
    const now = new Date();
    const hours = String(now.getHours()).padStart(2, '0');
    const minutes = String(now.getMinutes()).padStart(2, '0');
    const seconds = String(now.getSeconds()).padStart(2, '0');
    const timeString = `${hours}:${minutes}:${seconds}`;
    document.getElementById('time').textContent = timeString;
}

function updateDate() {
    const now = new Date();
    const options = { day: 'numeric', month: 'long', year: 'numeric' };
    const dateString = now.toLocaleDateString('pt-BR', options);
    document.getElementById('date').textContent = dateString;
}

// Atualiza a hora a cada segundo
setInterval(updateTime, 1000);

// Atualiza a data uma vez ao carregar a página
updateDate();

// Chama a função updateTime quando carrega a página pra garantir que a hora esteja correta
updateTime();
