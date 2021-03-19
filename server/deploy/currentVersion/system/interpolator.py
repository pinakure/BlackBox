class Interpolator:
    pool = []

    def __init__(self, initial_value, final_value, time, change_fallback):
        self.change_fallback = change_fallback
        self.total_time = time
        self.time = 1 if time <= 0 else time
        self.value = initial_value
        self.final_value = final_value
        self.polarity = 1.0 if final_value > initial_value else - 1 if final_value < initial_value else 0
        self.range = max(final_value, initial_value) - min(final_value, initial_value)

    def __update(self, delta):
        # returns true when finished the interpolation
        self.value+= (self.range / self.time) * self.polarity
        if self.polarity > 0:
            if self.value >= self.final_value:
                self.change_fallback(self.value)
                return True
        elif self.polarity < 0:
            if self.value <= self.final_value:
                self.change_fallback(self.value)
                return True
        self.change_fallback(self.value)
        return False

    @staticmethod
    def add(initial_value, final_value, time, change_fallback):
        Interpolator.pool.append(Interpolator(initial_value, final_value, time, change_fallback))
        
    @staticmethod
    def update(delta):
        pool = []
        for i in Interpolator.pool:
            if not i.__update(delta):pool.append(i)
        Interpolator.pool = pool
            
