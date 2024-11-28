import requests
from bs4 import BeautifulSoup
from collections import deque


class BFSator:

    @staticmethod
    def get_links(url):
        try:
            response = requests.get(url, timeout=10)
            response.raise_for_status()
            soup = BeautifulSoup(response.text, 'html.parser')
            links = set()
            for a_tag in soup.find_all('a', href=True):
                link = a_tag['href']
                # Upravte relativní odkazy na absolutní
                if link.startswith('/'):
                    link = requests.compat.urljoin(url, link)
                # Přidávejte pouze validní HTTP(S) odkazy
                if link.startswith('http'):
                    links.add(link)
            return links
        except Exception as e:
            return set()

    @staticmethod
    def get_base_url(url: str) -> str:
        # remove https://
        url = url.replace('https://', '')
        
        # remove www.
        if url.startswith('www.'):
            url = url[4:]

        # remove everything after first /
        base_url = url.split('/')[0]
        return base_url

    def __init__(self, start_url, target_url):
        self.start_url = start_url
        self.target_url = target_url
        self.visited = set()

    def bfs(self, current_level_que, next_level_que):
        while current_level_que:
            path = current_level_que.popleft()
            current_url = path[-1]
            current_base_url = BFSator.get_base_url(current_url)

            if current_url == self.target_url:
                return path
            
            if current_url not in self.visited:
                self.visited.add(current_url)
                print(f"Visiting: {current_url}")

                try:
                    links = BFSator.get_links(current_url)
                    for link in links:
                        if link[-1] == '/':
                            link = link[:-1]
                        base_link_url = BFSator.get_base_url(link)

                        new_path = list(path)
                        new_path.append(link)

                        if link == self.target_url:
                            return new_path
                        
                        if link in self.visited:
                            continue

                        # prefer other base urls
                        if current_base_url == base_link_url:                    
                            next_level_que.append(new_path)
                        else:
                            next_level_que.appendleft(new_path)
                except Exception as e:
                    pass

        return None

    def find_path_bfs(self):
        first_queue = deque([[self.start_url]])
        queues = deque([first_queue])

        current_bfs_level = 0

        while queues:
            print(f'\nStarting BFS level {current_bfs_level}\n{"="*40}')

            # get current bfs level queue
            current_level_que = queues.popleft()
            
            next_level_que = deque()
            path = self.bfs(current_level_que, next_level_que)
            if path:
                return path
            elif next_level_que:
                queues.append(next_level_que)
                current_bfs_level += 1
            else:
                break 

        return None
