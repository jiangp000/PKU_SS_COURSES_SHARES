import torch
from torch import nn
from torch.utils.data import TensorDataset, DataLoader
from Model import Model
import gym
from collections import deque
import random

# Parameters
use_cuda = True
episode_limit = 100
target_update_delay = 2  # update target net every target_update_delay episodes
test_delay = 10
learning_rate = 1e-4
epsilon = 1  # initial epsilon
min_epsilon = 0.1
epsilon_decay = 0.9 / 2.5e3
gamma = 0.99
memory_len = 10000

env = gym.make('CartPole-v1')
n_features = len(env.observation_space.high)
n_actions = env.action_space.n

memory = deque(maxlen=memory_len)
# each memory entry is in form: (state, action, env_reward, next_state)
device = torch.device("cuda" if use_cuda and torch.cuda.is_available() else "cpu")
criterion = nn.MSELoss()
policy_net = Model(n_features, n_actions).to(device)
target_net = Model(n_features, n_actions).to(device)
target_net.load_state_dict(policy_net.state_dict())
target_net.eval()


def get_states_tensor(sample, states_idx):
    sample_len = len(sample)
    states_tensor = torch.empty((sample_len, n_features), dtype=torch.float32, requires_grad=False)

    features_range = range(n_features)
    for i in range(sample_len):
        for j in features_range:
            states_tensor[i, j] = sample[i][states_idx][j].item()

    return states_tensor


def normalize_state(state):
    state[0] /= 2.5
    state[1] /= 2.5
    state[2] /= 0.3
    state[3] /= 0.3


def state_reward(state, env_reward):
    return env_reward - (abs(state[0]) + abs(state[2])) / 2.5


def get_action(state, e=min_epsilon):
    if random.random() < e:
        # explore
        action = random.randrange(0, n_actions)
    else:
        state = torch.tensor(state, dtype=torch.float32, device=device)
        action = policy_net(state).argmax().item()

    return action


def fit(model, inputs, labels):
    inputs = inputs.to(device)
    labels = labels.to(device)
    train_ds = TensorDataset(inputs, labels)
    train_dl = DataLoader(train_ds, batch_size=5)

    optimizer = torch.optim.Adam(params=model.parameters(), lr=learning_rate)
    model.train()
    total_loss = 0.0

    for x, y in train_dl:
        out = model(x)
        loss = criterion(out, y)
        total_loss += loss.item()
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    model.eval()

    return total_loss / len(inputs)


def optimize_model(train_batch_size=100):
    train_batch_size = min(train_batch_size, len(memory))
    train_sample = random.sample(memory, train_batch_size)

    state = get_states_tensor(train_sample, 0)
    next_state = get_states_tensor(train_sample, 3)

    q_estimates = policy_net(state.to(device)).detach()
    next_state_q_estimates = target_net(next_state.to(device)).detach()

    for i in range(len(train_sample)):
        q_estimates[i][train_sample[i][1]] = (state_reward(next_state[i], train_sample[i][2]) +
                                              gamma * next_state_q_estimates[i].max())

    fit(policy_net, state, q_estimates)


def train_one_episode():
    global epsilon
    current_state = env.reset()
    normalize_state(current_state)
    done = False
    score = 0
    reward = 0
    while not done:
        action = get_action(current_state, epsilon)
        next_state, env_reward, done, _ = env.step(action)
        normalize_state(next_state)
        memory.append((current_state, action, env_reward, next_state))
        current_state = next_state
        score += env_reward
        reward += state_reward(next_state, env_reward)

        optimize_model(100)

        epsilon -= epsilon_decay

    return score, reward


def test():
    state = env.reset()
    normalize_state(state)
    done = False
    score = 0
    reward = 0
    while not done:
        action = get_action(state)
        state, env_reward, done, _ = env.step(action)
        normalize_state(state)
        score += env_reward
        reward += state_reward(state, env_reward)

    return score, reward


def main():
    best_test_reward = 0

    for i in range(episode_limit):
        score, reward = train_one_episode()

        print(f'Episode {i + 1}: score: {score} - reward: {reward}')

        if i % target_update_delay == 0:
            target_net.load_state_dict(policy_net.state_dict())
            target_net.eval()

        if (i + 1) % test_delay == 0:
            test_score, test_reward = test()
            print(f'Test Episode {i + 1}: test score: {test_score} - test reward: {test_reward}')
            if test_reward > best_test_reward:
                print('New best test reward. Saving model')
                best_test_reward = test_reward
                torch.save(policy_net.state_dict(), 'policy_net.pth')

    if episode_limit % test_delay != 0:
        test_score, test_reward = test()
        print(f'Test Episode {episode_limit}: test score: {test_score} - test reward: {test_reward}')
        if test_reward > best_test_reward:
            print('New best test reward. Saving model')
            best_test_reward = test_reward
            torch.save(policy_net.state_dict(), 'policy_net.pth')

    print(f'best test reward: {best_test_reward}')


if __name__ == '__main__':
    main()