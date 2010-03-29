#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

class AtomicCounter
{
	public:
		AtomicCounter(int);
		~AtomicCounter();

		operator int() const;
		const AtomicCounter &operator +=(int);
		const AtomicCounter &operator -=(int);
		const AtomicCounter &operator ++(int);
		const AtomicCounter &operator --(int);

	private:
		const AtomicCounter &operator =
		int counter;
};

#endif
