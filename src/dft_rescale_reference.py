import numpy as np

def DFT_rescale(x, factor):
  X = np.fft.fft(x)
  breakpoint()
  # separate even and odd lengths
  parity = (len(X) % 2 == 0)
  N = int(len(X) / 2) + 1 if parity else int((len(X) + 1) / 2)
  Y = np.zeros(N, dtype=np.complex128)
  # work only in the first half of the DFT vector since input is real
  for n in range(0, N):
    # accumulate original frequency bins into rescaled bins
    ix = int(n * factor)
    if ix < N:
      Y[ix] += X[n]
    else:
      break
  # now rebuild a Hermitian-symmetric DFT
  Y = np.r_[Y, np.conj(Y[-2:0:-1])] if parity else np.r_[Y, np.conj(Y[-1:0:-1])]
  return np.real(np.fft.ifft(Y)) 

def main():
  num_samples = 20

  # Generate an array of 20 equally spaced points between 0 and 2π
  t = np.linspace(0, 2*np.pi, num_samples, endpoint=False)

  # Compute the values of the sinusoid at those points
  x = np.sin(t)
  DFT_rescale(x, 2)

if __name__ == "__main__":
  main()
