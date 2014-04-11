package mallory

import (
	"bytes"
	"crypto/rand"
	"crypto/rsa"
	"crypto/tls"
	"crypto/x509"
	"encoding/pem"
	"errors"
	"sync"
)

// certificate and private key pair
// signee is a template to be signed by signer, self-signed is not supported
func CreateSignedCert(signee, signer *x509.Certificate) (cert *tls.Certificate, err error) {
	if signee == signer {
		err = errors.New("Signee and Signer can not be the same")
		return
	}

	bits := signer.PublicKey.(*rsa.PublicKey).N.BitLen()
	key, err := rsa.GenerateKey(rand.Reader, bits)
	if err != nil {
		return
	}

	var keypem bytes.Buffer
	err = pem.Encode(&keypem, &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS1PrivateKey(key),
	})
	if err != nil {
		return
	}

	crtder, err := x509.CreateCertificate(rand.Reader, signee, signer, &key.PublicKey, key)
	if err != nil {
		return
	}

	var crtpem bytes.Buffer
	err = pem.Encode(&crtpem, &pem.Block{Type: "CERTIFICATE", Bytes: crtder})
	if err != nil {
		return
	}

	crt, err := tls.X509KeyPair(crtpem.Bytes(), keypem.Bytes())
	return &crt, err
}

// certificates pool, able to thread safely add and fetch
type CertPool struct {
	data  map[string]*tls.Certificate
	mutex sync.RWMutex
}

func NewCertPool() *CertPool {
	pool := &CertPool{
		data: make(map[string]*tls.Certificate),
	}
	return pool
}

func (self *CertPool) AddSafe(key string, cert *tls.Certificate) {
	self.mutex.Lock()
	self.data[key] = cert
	self.mutex.Unlock()
}

func (self *CertPool) GetSafe(key string) *tls.Certificate {
	self.mutex.RLock()
	cert, ok := self.data[key]
	self.mutex.RUnlock()
	if !ok {
		return nil
	}
	return cert
}
