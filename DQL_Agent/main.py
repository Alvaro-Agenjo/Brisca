import DQL
import torch

# 1. Instanciar el agente (asegúrate de que el constructor inicialice las redes)
agente = DQL.Agent()

# 2. Cargar el estado actual (esto suele cargar Q_eval)
agente.load_model("brisca_v10_20000.pth")

# 3. Sincronizar las redes
# Nota: Si Q_target y Q_eval son objetos torch.nn.Module expuestos:
agente.Q_target.load_state_dict(agente.Q_eval.state_dict())

# 4. Guardar el modelo corregido
agente.save_model("brisca_v11", 20000)
print("Sincronización completada y modelo guardado como brisca_v11_20000.pth")