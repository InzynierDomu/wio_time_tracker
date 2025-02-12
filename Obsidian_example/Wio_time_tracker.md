```dataviewjs
const pages = dv.pages('"Wio_time_tracker"')
const Names = pages.map(p => p.file.name).values
const work_Praca = pages.map(p => p.work_work_1).values
const meeting_Praca = pages.map(p => p.meeting_work).values

const chartData = {
    type: 'bar',
    data: {
        labels: Names, // Etykiety osi X
        datasets: [
            {
                label: 'work', // Etykieta dla segmentu "Praca"
                data: work_Praca, // Dane dla "Praca"
                backgroundColor: 'rgba(255, 99, 132, 0.2)', // Kolor wypełnienia dla "Praca"
                borderColor: 'rgba(255, 99, 132, 1)', // Kolor obramowania dla "Praca"
                borderWidth: 1,
            },
            {
                label: 'meetings', // Etykieta dla segmentu "Spotkania"
                data: meeting_Praca, // Dane dla "Spotkania"
                backgroundColor: 'rgba(54, 162, 235, 0.2)', // Kolor wypełnienia dla "Spotkania"
                borderColor: 'rgba(54, 162, 235, 1)', // Kolor obramowania dla "Spotkania"
                borderWidth: 1,
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: {
                position: 'top', // Pozycja legendy
            },
        },
        scales: {
            x: {
                stacked: true, // Włączone układanie słupków w osi X
            },
            y: {
                stacked: true, // Włączone układanie słupków w osi Y
                beginAtZero: true, // Oś Y zaczyna od zera
            }
        }
    }
}

window.renderChart(chartData, this.container)
```

```dataviewjs
const pages = dv.pages('"Wio_time_tracker"')

const Names = pages.map(p => p.file.name).values
const work_Project = pages.map(p => p.work_project).values

const chartData = {
    type: 'line', // Wykres liniowy
    data: {
        labels: Names, // Etykiety osi X
        datasets: [
            {
                label: 'Project time', 
                data: work_Project, 
                borderColor: 'rgba(75, 192, 192, 1)', // Kolor linii
                backgroundColor: 'rgba(75, 192, 192, 0.2)', // Opcjonalne wypełnienie
                borderWidth: 2,
                fill: false, // Wyłącza wypełnienie pod linią
                tension: 0.3 // Wygładzenie linii
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: {
                position: 'top',
            },
        },
        scales: {
            y: {
                beginAtZero: true, // Skala Y zaczyna od 0
            }
        }
    }
}

window.renderChart(chartData, this.container)
```