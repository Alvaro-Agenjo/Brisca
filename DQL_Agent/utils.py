from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

AGENT_PATH = Path(__file__).resolve().parent.parent / "agents"
AGENT_PATH.mkdir(parents=True, exist_ok=True)


def smooth(arr, w=10):
    arr = np.array(arr, dtype=float)
    if len(arr) < w:
        return arr, np.arange(len(arr))
    kernel = np.ones(w) / w
    return np.convolve(arr, kernel, mode='valid'), np.arange(len(arr) - w + 1)

def plot_training(rewards, losses, epsilons, title="DQN Training Analysis", points=None):
    # Allow large datasets to render in Agg without overflow.
    plt.rcParams['agg.path.chunksize'] = 10000

    # Creamos una cuadrícula de 1 fila y 3 columnas para Recompensa, Loss y Epsilon
    fig, axes = plt.subplots(2, 2, figsize=(20, 20))
    
    # Definimos la configuración de cada gráfico: (datos, color, etiqueta Y, título, línea_objetivo)
    specs = [
        (rewards,  '#1565C0', 'Reward',       'Episode Reward',     None), # Azul
        (points,   '#FF8F00', 'Points',       'Points per game',    60  ), # Naranja
        (losses,   '#C62828', 'Huber Loss',   'Network Loss',       None), # Rojo
        (epsilons, '#6A1B9A', 'Epsilon',      'Exploration Rate',   None)  # Púrpura
    ]

    for ax, (data, color, ylabel, subtitle, hline) in zip(axes.flatten(), specs):
        if data is None:
            vals = np.array([], dtype=float)
        else:
            vals = np.array(data, dtype=float)
        ts = np.arange(1, len(vals) + 1)
        
        # if data is losses:
        #     ax.set_ylim(0, 80)
        # 1. Graficar datos originales con transparencia (el "ruido")
        ax.plot(ts, vals, alpha=0.25, color=color, lw=1)
        
        # 2. Graficar línea suavizada
        sm, idx = smooth(vals, w=20) # w=20 para un suavizado más marcado
        ax.plot(ts[idx], sm, color=color, lw=2.5, label='Suavizado')
        
        # 3. Añadir línea de "objetivo" si existe (ej. recompensa para resolver el entorno)
        if hline is not None:
            ax.axhline(hline, color='green', ls='--', lw=1.5, label=f'Meta >= {hline}')
            
        # Formato estético
        ax.set_xlabel('Timesteps')
        ax.set_ylabel(ylabel)
        ax.set_title(subtitle, fontweight='bold')
        ax.grid(alpha=0.3)
        ax.legend(fontsize=9)

    plt.suptitle(title, fontsize=16, fontweight='bold', y=1.05)
    plt.tight_layout()
    
    # Guardar y mostrar
    path = AGENT_PATH / (title + '.png')
    plt.savefig(path, bbox_inches='tight', dpi=300)
    # plt.show()
    plt.close()
