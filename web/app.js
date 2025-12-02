// Election Data Analysis System - Frontend JavaScript

const API_BASE = 'http://localhost:8080/api';

// State management
let availableCountries = [];

// Initialize the application
document.addEventListener('DOMContentLoaded', async () => {
    setupTabs();
    await loadCountries();
    setupEventListeners();
});

// Tab switching functionality
function setupTabs() {
    const tabButtons = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');
    
    tabButtons.forEach(button => {
        button.addEventListener('click', () => {
            const tabName = button.dataset.tab;
            
            // Remove active class from all tabs
            tabButtons.forEach(btn => btn.classList.remove('active'));
            tabContents.forEach(content => content.classList.remove('active'));
            
            // Add active class to clicked tab
            button.classList.add('active');
            document.getElementById(tabName).classList.add('active');
        });
    });
}

// Load available countries and years
async function loadCountries() {
    try {
        const response = await fetch(`${API_BASE}/countries`);
        const data = await response.json();
        availableCountries = data.countries;
        
        populateCountrySelects();
    } catch (error) {
        console.error('Error loading countries:', error);
        showError('Failed to load countries. Make sure the server is running.');
    }
}

// Populate all country select dropdowns
function populateCountrySelects() {
    const selects = ['stats-country', 'compare-country', 'cand-country'];
    
    selects.forEach(selectId => {
        const select = document.getElementById(selectId);
        select.innerHTML = '<option value="">Select Country</option>';
        
        availableCountries.forEach(country => {
            const option = document.createElement('option');
            option.value = country.name;
            option.textContent = country.name;
            select.appendChild(option);
        });
    });
}

// Setup event listeners
function setupEventListeners() {
    // Stats tab
    document.getElementById('stats-country').addEventListener('change', (e) => {
        updateYearSelect('stats-year', e.target.value);
    });
    document.getElementById('stats-analyze').addEventListener('click', analyzeElection);
    
    // Compare tab
    document.getElementById('compare-country').addEventListener('change', (e) => {
        updateYearSelect('compare-year1', e.target.value);
        updateYearSelect('compare-year2', e.target.value);
    });
    document.getElementById('compare-analyze').addEventListener('click', compareElections);
    
    // Candidates tab
    document.getElementById('cand-country').addEventListener('change', (e) => {
        updateYearSelect('cand-year', e.target.value);
    });
    document.getElementById('cand-analyze').addEventListener('click', showTopCandidates);
}

// Update year select based on country
function updateYearSelect(selectId, countryName) {
    const select = document.getElementById(selectId);
    select.innerHTML = '<option value="">Select Year</option>';
    
    const country = availableCountries.find(c => c.name === countryName);
    if (country) {
        country.years.forEach(year => {
            const option = document.createElement('option');
            option.value = year;
            option.textContent = year;
            select.appendChild(option);
        });
    }
}

// Analyze election statistics
async function analyzeElection() {
    const country = document.getElementById('stats-country').value;
    const year = document.getElementById('stats-year').value;
    
    if (!country || !year) {
        alert('Please select both country and year');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/stats?country=${encodeURIComponent(country)}&year=${year}`);
        const data = await response.json();
        
        displayElectionStats(data);
    } catch (error) {
        console.error('Error analyzing election:', error);
        showError('Failed to analyze election data');
    }
}

// Display election statistics
function displayElectionStats(data) {
    // Update stat cards
    document.getElementById('total-votes').textContent = data.totalVotes.toLocaleString();
    document.getElementById('total-seats').textContent = data.totalSeats;
    document.getElementById('total-candidates').textContent = data.totalCandidates;
    document.getElementById('total-constituencies').textContent = data.constituencies;
    
    // Update party table
    const tbody = document.querySelector('#party-table tbody');
    tbody.innerHTML = '';
    
    data.parties.forEach(party => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td><strong>${escapeHtml(party.party)}</strong></td>
            <td>${party.totalVotes.toLocaleString()}</td>
            <td>${party.voteShare.toFixed(2)}%</td>
            <td>${party.seatsWon}</td>
            <td>${party.candidatesCount}</td>
        `;
        tbody.appendChild(row);
    });
    
    // Show results
    document.getElementById('stats-results').classList.remove('hidden');
    
    // Scroll to results
    document.getElementById('stats-results').scrollIntoView({ behavior: 'smooth' });
}

// Compare two elections
async function compareElections() {
    const country = document.getElementById('compare-country').value;
    const year1 = document.getElementById('compare-year1').value;
    const year2 = document.getElementById('compare-year2').value;
    
    if (!country || !year1 || !year2) {
        alert('Please select country and both years');
        return;
    }
    
    if (year1 === year2) {
        alert('Please select two different years');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/compare?country=${encodeURIComponent(country)}&year1=${year1}&year2=${year2}`);
        const data = await response.json();
        
        displayComparison(data);
    } catch (error) {
        console.error('Error comparing elections:', error);
        showError('Failed to compare elections');
    }
}

// Display comparison results
function displayComparison(data) {
    // Update stat cards
    const voteChange = data.voteChange;
    const voteChangePercent = data.voteChangePercent;
    
    document.getElementById('vote-change').innerHTML = 
        `<span class="${voteChange >= 0 ? 'positive' : 'negative'}">${voteChange >= 0 ? '+' : ''}${voteChange.toLocaleString()}</span>`;
    
    document.getElementById('vote-change-percent').innerHTML = 
        `<span class="${voteChangePercent >= 0 ? 'positive' : 'negative'}">${voteChangePercent >= 0 ? '+' : ''}${voteChangePercent.toFixed(2)}%</span>`;
    
    // Update comparison table
    const tbody = document.querySelector('#compare-table tbody');
    tbody.innerHTML = '';
    
    // Sort by absolute vote change
    const sortedParties = data.partyChanges.sort((a, b) => Math.abs(b.voteChange) - Math.abs(a.voteChange));
    
    sortedParties.forEach(party => {
        const row = document.createElement('tr');
        const voteChangeClass = party.voteChange >= 0 ? 'positive' : 'negative';
        const seatChangeClass = party.seatChange >= 0 ? 'positive' : 'negative';
        
        row.innerHTML = `
            <td><strong>${escapeHtml(party.party)}</strong></td>
            <td class="${voteChangeClass}">${party.voteChange >= 0 ? '+' : ''}${party.voteChange.toLocaleString()}</td>
            <td class="${seatChangeClass}">${party.seatChange >= 0 ? '+' : ''}${party.seatChange}</td>
        `;
        tbody.appendChild(row);
    });
    
    // Show results
    document.getElementById('compare-results').classList.remove('hidden');
    
    // Scroll to results
    document.getElementById('compare-results').scrollIntoView({ behavior: 'smooth' });
}

// Show top candidates
async function showTopCandidates() {
    const country = document.getElementById('cand-country').value;
    const year = document.getElementById('cand-year').value;
    const count = document.getElementById('cand-count').value;
    
    if (!country || !year) {
        alert('Please select both country and year');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/top-candidates?country=${encodeURIComponent(country)}&year=${year}&n=${count}`);
        const data = await response.json();
        
        displayTopCandidates(data.candidates);
    } catch (error) {
        console.error('Error loading candidates:', error);
        showError('Failed to load top candidates');
    }
}

// Display top candidates
function displayTopCandidates(candidates) {
    const tbody = document.querySelector('#cand-table tbody');
    tbody.innerHTML = '';
    
    candidates.forEach((candidate, index) => {
        const row = document.createElement('tr');
        const electedBadge = candidate.elected 
            ? '<span class="elected-badge elected-yes">Yes</span>'
            : '<span class="elected-badge elected-no">No</span>';
        
        row.innerHTML = `
            <td><strong>${index + 1}</strong></td>
            <td>${escapeHtml(candidate.candidate)}</td>
            <td>${escapeHtml(candidate.party)}</td>
            <td>${escapeHtml(candidate.constituency)}</td>
            <td>${candidate.votes.toLocaleString()}</td>
            <td>${electedBadge}</td>
        `;
        tbody.appendChild(row);
    });
    
    // Show results
    document.getElementById('cand-results').classList.remove('hidden');
    
    // Scroll to results
    document.getElementById('cand-results').scrollIntoView({ behavior: 'smooth' });
}

// Utility functions
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

function showError(message) {
    alert(message);
}
