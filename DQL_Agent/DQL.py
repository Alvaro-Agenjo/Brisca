import torch as T
T.backends.cudnn.benchmark = True
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import numpy as np


import os
import utils as utils
from utils import AGENT_PATH

DEBUG = False

# Usamos una función que llamaremos DESDE C++ para asegurar que el canal está abierto
def activar_debug():
    import debugpy

    if not debugpy.is_client_connected():
        debugpy.listen(5678)
        print("Esperando conexión en el puerto 5678...")
        debugpy.wait_for_client()
        print("Python Debugger conectado con éxito.")

def breakpoint(active = False):
    if active and DEBUG:
        import debugpy
        debugpy.breakpoint() 


class DeepQNetwork(nn.Module):
    def __init__(self, learning_rate, input_dims, fc1_dims, fc2_dims, n_actions):
        super(DeepQNetwork, self).__init__()
        
        # asignacion de variables internas
        self.input_dims = input_dims
        self.fc1_dims = fc1_dims
        self.fc2_dims = fc2_dims
        self.n_actions = n_actions

        # definicion de las capas y propiedades
        self.fc1 = nn.Linear(*self.input_dims, self.fc1_dims)
        self.fc2 = nn.Linear(self.fc1_dims, self.fc2_dims)
        self.fc3 = nn.Linear(self.fc2_dims, self.n_actions)

        self.optimizer = optim.Adam(self.parameters(), lr = learning_rate)
        # self.loss = nn.MSELoss()
        self.loss = nn.HuberLoss()
        self.device = T.device('cuda:0' if T.cuda.is_available() else 'cpu')
        self.to(self.device)

    def forward(self, state):
        x = F.relu(self.fc1(state))
        x = F.relu(self.fc2(x))
        actions = self.fc3(x)
        
        return actions
    
class Agent():
    def __init__(self, learning_rate, gamma, epsilon_init, input_dims, batch_size, n_actions, fc1_dim=256, fc2_dim=256,
                 update_steps = 1000, max_mem_size=100000, eps_end=0.01, eps_dec= 5e-4):
        self.gamma = gamma
        self.epsilon = epsilon_init
        self.eps_min = eps_end
        self.eps_dec = eps_dec
        self.learning_rate = learning_rate
        self.action_space = [ i for i in range(n_actions)]
        self.mem_size = max_mem_size
        self.batch_size= batch_size
        self.update_steps = update_steps
        self.mem_counter = 0

        self.Q_eval = DeepQNetwork(self.learning_rate, n_actions=n_actions, input_dims=input_dims,
                                   fc1_dims=fc1_dim, fc2_dims=fc2_dim)
        self.Q_target = DeepQNetwork(self.learning_rate, n_actions=n_actions, input_dims=input_dims,
                                   fc1_dims=fc1_dim, fc2_dims=fc2_dim)
        self.Q_target.load_state_dict(self.Q_eval.state_dict())
    
        
        
        self.state_memory = np.zeros((self.mem_size, *input_dims), dtype=np.float32)
        self.new_state_memory = np. zeros((self.mem_size, *input_dims), dtype=np.float32)
        self.action_memory = np. zeros(self.mem_size, dtype=np.int32)
        self.reward_memory = np. zeros(self.mem_size, dtype=np.float32)
        self.terminal_memory = np.zeros(self.mem_size, dtype=np.bool_)

        self.history_rewards = []
        self.history_loss =[]
        self.history_epsilon = []
        self.history_points = []
        self.temp_reward = 0

        self.decision_counter = 0

    def store_transition(self, state, action, reward, state_, done):
        index = self.mem_counter % self.mem_size
        self.state_memory[index] = state
        self.new_state_memory[index] = state_
        self.action_memory[index]= action
        self.reward_memory[index] = reward
        self.terminal_memory[index] = done
        self.mem_counter +=1

        self.temp_reward += reward
        if done:
            self.history_rewards.append(self.temp_reward)
            self.temp_reward = 0

    def record_episode_points(self, points):
        try:
            self.history_points.append(float(points))
        except Exception:
            self.history_points.append(0.0)

    def choose_action(self, observation, cards):
        if np.random.random() > self.epsilon:
            state = T.tensor([observation], dtype=T.float32).to(self.Q_eval.device)
            with T.no_grad():
                actions = self.Q_eval.forward(state)

            if cards < self.Q_eval.n_actions:
                mask = T.full_like(actions, float('-inf'))
                mask[0, :cards] = 0.0
                actions = actions + mask

            action = int(T.argmax(actions).item())
        else:
            action = int(np.random.choice(range(cards)))

        return action

    def learn(self):
        if self.mem_counter < self.batch_size:
            return

        self.Q_eval.optimizer.zero_grad()
        max_mem = min(self.mem_counter, self.mem_size)
        batch = np.random.choice(max_mem, self.batch_size, replace=False)

        batch_index = np.arange(self.batch_size, dtype=np.int32)

        state_batch = T.tensor(self.state_memory[batch]).to(self.Q_eval.device)
        new_state_batch = T.tensor(self.new_state_memory[batch]).to(self.Q_eval.device)
        reward_batch = T.tensor(self.reward_memory[batch]).to(self.Q_eval.device)
        terminal_batch = T.tensor(self.terminal_memory[batch]).to(self.Q_eval.device)
        action_batch = T.tensor(self.action_memory[batch], dtype=T.long).to(self.Q_eval.device)

        q_eval = self.Q_eval.forward(state_batch)[batch_index, action_batch]
        with T.no_grad():
            q_next = self.Q_target.forward(new_state_batch)
        q_next[terminal_batch] = 0.0

        q_target = reward_batch + self.gamma * T.max(q_next, dim=1)[0]
        loss = self.Q_eval.loss(q_target, q_eval).to(self.Q_eval.device)
        loss.backward()
        T.nn.utils.clip_grad_norm_(self.Q_eval.parameters(), 1.0)
        self.Q_eval.optimizer.step()

        self.history_epsilon.append(self.epsilon)
        self.history_loss.append(loss.item())

        self.epsilon = self.epsilon - self.eps_dec if self.epsilon > self.eps_min else self.eps_min

        self.decision_counter += 1
        if self.decision_counter >= self.update_steps:
            self.decision_counter = 0
            self.Q_target.load_state_dict(self.Q_eval.state_dict())

        return loss.item()
    
    def save_model(self, name, step):
        filename = f"{name}_{step}.pth"
        path = AGENT_PATH / filename
        checkpoint = {
            'q_eval_state_dict': self.Q_eval.state_dict(),
            'q_target_state_dict': self.Q_target.state_dict(),
            'optimizer_state_dict': self.Q_eval.optimizer.state_dict()
        }
        T.save(checkpoint, path)
        print(f"... checkpoint guardado en {path} ...")

    def load_model(self, filename="brisca_model.pth"):
        path = AGENT_PATH / filename
        
        if path.exists():
            checkpoint = T.load(path, map_location=self.Q_eval.device)
            
            self.Q_eval.load_state_dict(checkpoint['q_eval_state_dict'])
            self.Q_target.load_state_dict(checkpoint['q_target_state_dict'])
            self.Q_eval.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
            print(f"Modelo cargado desde: {path}")
        else:
            raise ValueError(f" No se encontró checkpoint {filename}.")
        
    def show_training(self, name, episode):
        title = name + "_" + str(episode)
        utils.plot_training( self.history_rewards,  self.history_loss,  self.history_epsilon, title=title, points=self.history_points)