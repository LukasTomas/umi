
class Solver:
    VALUE_NOT_SET = -1

    def __init__(self, map_data, neighbours, colors_number):
        self.map_data = map_data
        self.neighbours = neighbours
        self._values = [Solver.VALUE_NOT_SET] * self.map_data.shape[0]
        self.colors_number = colors_number  # Define the number of colors here

    def bj(self):
        """Main function to execute backjumping algorithm."""
        conflict_set = [set() for _ in range(self.map_data.shape[0])]
        if self._bj(self.map_data.shape[0] - 1, conflict_set):
            self.map_data['value'] = self._values
            return True
        return False

    def bt(self):
        """Runs backtracking algorithm as a fallback option."""
        if self._bt(self.map_data.shape[0] - 1):
            self.map_data['value'] = self._values
            return True
        return False
    
    def mac_bt(self):
        """Main function to execute MAC with backtracking."""
        domains = [set(range(self.colors_number)) for _ in range(self.map_data.shape[0])]
        if self._mac_bt(self.map_data.shape[0] - 1, domains):
            self.map_data['value'] = self._values
            return True
        return False

    def _mac_bt(self, n, domains):
        """MAC-BT with AC-3 for arc consistency checking before assignments."""
        if n < 0:
            return self._is_valid()

        original_domains = [d.copy() for d in domains]  # Copy the original domains
        for color in domains[n].copy():
            self._values[n] = color
            if self._ac3(n, domains):
                if self._mac_bt(n - 1, domains):
                    return True
            domains = [d.copy() for d in original_domains]  # Restore domains for the next color

        self._values[n] = Solver.VALUE_NOT_SET
        return False

    def _ac3(self, var, domains):
        """Performs AC-3 to ensure arc consistency."""
        queue = [(var, neighbour) for neighbour in self.neighbours.loc[var]['neighbours']]
        
        while queue:
            (xi, xj) = queue.pop(0)
            if self._revise(xi, xj, domains):
                if not domains[xi]:  # If a domain is empty, inconsistency found
                    return False
                # Enqueue all neighbors of xi excluding xj
                for xk in self.neighbours.loc[xi]['neighbours']:
                    if xk != xj:
                        queue.append((xk, xi))
        return True

    def _revise(self, xi, xj, domains):
        """Revise xi's domain by removing values inconsistent with xj."""
        revised = False
        for x in domains[xi].copy():
            if not any(self._is_consistent(x, y) for y in domains[xj]):
                domains[xi].remove(x)
                revised = True
        return revised
    
    def _is_consistent(self, x, y):
        """Check if colors x and y are consistent (not the same color)."""
        return x != y

    def _bt(self, n):
        """Standard backtracking implementation."""
        if n < 0:
            return self._is_valid()

        for color in range(self.colors_number):
            self._values[n] = color
            if self._is_valid() and self._bt(n - 1):
                return True

        self._values[n] = Solver.VALUE_NOT_SET
        return False

    def _bj(self, n, conflict_set):
        """Backjumping algorithm with conflict set tracking."""
        if n < 0:
            return self._is_valid()

        for color in range(self.colors_number):
            self._values[n] = color

            if self._is_valid():
                if self._bj(n - 1, conflict_set):
                    return True
            else:
                conflicting_vars = self._find_conflicts(n)
                conflict_set[n].update(conflicting_vars)

        self._values[n] = Solver.VALUE_NOT_SET
        if conflict_set[n]:
            deepest_conflict = max(conflict_set[n])
            for i in range(deepest_conflict + 1, len(conflict_set)):
                conflict_set[i].discard(n)
            return self._bj(deepest_conflict, conflict_set)
        return False

    def _is_valid(self):
        """Check if current assignments are valid."""
        for i in range(len(self._values)):
            color = self._values[i]
            if color == Solver.VALUE_NOT_SET:
                continue

            neighbours = self.neighbours.loc[i]['neighbours']
            for neighbour in neighbours:
                neighbour_color = self._values[neighbour]
                if neighbour_color == Solver.VALUE_NOT_SET:
                    continue

                if color == neighbour_color:
                    return False
        return True

    def _find_conflicts(self, var):
        """Finds conflicting variables for a given variable."""
        conflicts = set()
        color = self._values[var]

        if color == Solver.VALUE_NOT_SET:
            return conflicts

        neighbours = self.neighbours.loc[var]['neighbours']
        for neighbour in neighbours:
            neighbour_color = self._values[neighbour]
            if neighbour_color == color:
                conflicts.add(neighbour)
        return conflicts

    def _format_number(self, number):
        """Formats a number with comma separators."""
        return f'{number:,}'.replace(',', ' ')
